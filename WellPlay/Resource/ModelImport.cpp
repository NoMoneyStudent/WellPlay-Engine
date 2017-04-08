#include "stdafx.h"
#include <set>
#include "Wnd\LogWnd.h"
#include "Utility\FileUtility.h"
#include "ResourceManager.h"
#include "EngineRuntime\GameObject.h"
#include "EngineRuntime\Transform.h"
#include "EngineRuntime\MeshRender.h"
#include "EngineRuntime\SkinMeshRender.h"
#include "EngineRuntime\Animator.h"
#include "ModelImport.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Serialize.h"

using namespace std;

vector<pair<weak_ptr<SkinMeshRender>, Avatar*>> setupAvatar;
shared_ptr<Assets> asset;

void ProcessNode(const aiScene* scene, shared_ptr<GameObject> model, aiNode* node);
void ProcessAnimation(const aiScene* scene);

void SetVertexVector(XMVECTOR& vector1, aiVector3D& vector2)
{
	vector1.m128_f32[0] = vector2.x;
	vector1.m128_f32[1] = vector2.y;
	vector1.m128_f32[2] = vector2.z;
}

void SetVertexVector(XMFLOAT2& vector1, aiVector3D& vector2)
{
	vector1.x = vector2.x;
	vector1.y = vector2.y;
}

void SetVertexVector(XMFLOAT3& vector1, aiVector3D& vector2)
{
	vector1.x = vector2.x;
	vector1.y = vector2.y;
	vector1.z = vector2.z;
}

void SetVertexVector(XMFLOAT4& vector1, aiQuaternion& vector2)
{
	vector1.x = vector2.x;
	vector1.y = vector2.y;
	vector1.z = vector2.z;
	vector1.w = vector2.w;
}

void SetVertexVector(XMFLOAT4& vector1, aiColor4D& vector2)
{
	vector1.x = vector2.r;
	vector1.y = vector2.g;
	vector1.z = vector2.b;
	vector1.w = vector2.a;
}

void CopyMatrix(XMFLOAT4X4& m1,aiMatrix4x4& m2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m1.m[i][j] = m2[i][j];
		}
	}
}

void SetQuater(XMVECTOR& vector1, aiQuaternion vector2)
{
	vector1.m128_f32[0] = vector2.x;
	vector1.m128_f32[1] = vector2.y;
	vector1.m128_f32[2] = vector2.z;
	vector1.m128_f32[3] = vector2.w;
}

void ReadVertexUtility(CommonVertex& v,aiMesh* mesh,int index)
{
	aiVector3D vector = mesh->mVertices[index];
	SetVertexVector(v.pos, vector);
	if (mesh->HasNormals())
	{
		vector = mesh->mNormals[index];
		SetVertexVector(v.normal, vector);
	}
	if (mesh->HasTangentsAndBitangents())
	{
		vector = mesh->mTangents[index];
		SetVertexVector(v.tangent, vector);
	}
	if (mesh->HasVertexColors(0))
	{
		aiColor4D col = mesh->mColors[0][index];
		SetVertexVector(v.color, col);
	}
	if (mesh->HasTextureCoords(0))
	{
		vector = mesh->mTextureCoords[0][index];
		SetVertexVector(v.UV0, vector);
	}
	if (mesh->HasTextureCoords(1))
	{
		vector = mesh->mTextureCoords[1][index];
		SetVertexVector(v.UV1, vector);
	}
}

void ReshIndicesUtility(vector<UINT>& I, aiMesh* mesh)
{
	for (UINT j = 0; j < mesh->mNumFaces; j++)
	{
		for (UINT k = 0; k < mesh->mFaces[j].mNumIndices; k++)
		{
			I.push_back(mesh->mFaces[j].mIndices[k]);
		}
	}
}

void ProcessNode(const aiScene* scene, shared_ptr<GameObject> parent, aiNode* node)
{
	shared_ptr<GameObject> myNode = GameObject::Instantiate(node->mName.C_Str());
	if (parent != nullptr)
	{
		myNode->GetTransform().lock()->SetParent(parent->GetTransform().lock());
	}
	weak_ptr<Transform> myTrans = myNode->GetTransform();
	aiVector3D aiS;
	aiVector3D aiT;
	aiQuaternion aiR;
	node->mTransformation.Decompose(aiS, aiR, aiT);
	XMVECTOR vc;
	SetVertexVector(vc, aiT);
	myTrans.lock()->SetLocalPosition(vc);
	SetVertexVector(vc, aiS);
	myTrans.lock()->SetLocalScale(vc);
	SetQuater(vc, aiR);
	myTrans.lock()->SetLocalRotation(vc);

	EditorWindows::LogWnd::Print(L"生成节点" + MakeWStr(node->mName.C_Str()) + L"   父节点" + ((parent == nullptr) ? L"" : MakeWStr(parent->GetName())));
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		string meshname = node->mName.C_Str();
		meshname += "Mesh";
		meshname += to_string(i);
		if (!ResourceManager::checkifExit(meshname)&& mesh->mNumVertices>0)
		{
			if (mesh->HasBones())
			{
				shared_ptr<AniMesh> meshdata(new AniMesh());

				meshdata->name = meshname;
				vector<AniVertex> V(mesh->mNumVertices);
				for (UINT j = 0; j < mesh->mNumVertices; j++)
				{
					ReadVertexUtility(V[j], mesh, j);
				}
				meshdata->vertexs = std::move(V);

				vector<UINT> I;
				ReshIndicesUtility(I, mesh);
				meshdata->indexs = std::move(I);

				shared_ptr<Avatar>  avatar(new Avatar());
				for (UINT j = 0; j < mesh->mNumBones; j++)
				{
					Bone bone;
					aiBone* aibone = mesh->mBones[j];
					bone.name = aibone->mName.C_Str();
					CopyMatrix(bone.Bind, aibone->mOffsetMatrix.Transpose());
					auto index = avatar->bonelists.size();
					for (UINT k = 0; k < aibone->mNumWeights; k++)
					{
						int id = aibone->mWeights[k].mVertexId;
						for (int q = 0; q < 4; q++)
						{
							if (meshdata->vertexs[id].BoneIndex[q] == 65536)
							{
								meshdata->vertexs[id].BoneIndex[q] = index;
								meshdata->vertexs[id].BoneWeights[q] = aibone->mWeights[k].mWeight;
								break;
							}
						}
					}
					avatar->bonelists.push_back(bone);
				}
				avatar->name = meshdata->name;
				ResourceManager::AddAvatar(avatar);
				asset->avatarlist.push_back(avatar);
				ResourceManager::AddMesh(static_pointer_cast<Mesh>(meshdata),true);
				asset->meshlist.push_back(make_pair(static_pointer_cast<Mesh>(meshdata),true));
			}
			else
			{
				shared_ptr<CommonMesh> meshdata(new CommonMesh);
				meshdata->name = meshname;
				vector<CommonVertex> V(mesh->mNumVertices);
				for (UINT j = 0; j < mesh->mNumVertices; j++)
				{
					ReadVertexUtility(V[j], mesh, j);
				}
				meshdata->vertexs = std::move(V);
				vector<UINT> I;
				ReshIndicesUtility(I, mesh);
				meshdata->indexs = std::move(I);

				ResourceManager::AddMesh(static_pointer_cast<Mesh>(meshdata), false);
				asset->meshlist.push_back(make_pair(static_pointer_cast<Mesh>(meshdata),false));
			}
		}
		if (mesh->HasBones())
		{
			auto render = myNode->AddComponent<SkinMeshRender>();
			render.lock()->SetMesh(ResourceManager::GetMesh(meshname));
			setupAvatar.push_back(make_pair(render, ResourceManager::GetAvatar(meshname)));

			auto ani=myNode->AddComponent<Animator>();
		}
		else
		{
			auto render = myNode->AddComponent<MeshRender>();
			render.lock()->SetMesh(ResourceManager::GetMesh(meshname));
		}
	}
	
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(scene, myNode, node->mChildren[i]);
	}
}

void ProcessAnimation(const aiScene* scene)
{
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* ani = scene->mAnimations[i];
		shared_ptr<AnimationClip> clip(new AnimationClip());
		clip->name = ani->mName.C_Str();
		double ticks = ani->mTicksPerSecond == 0 ? 1 : ani->mTicksPerSecond;
		clip->durning = ani->mDuration / ticks;
		for (UINT j = 0; j < ani->mNumChannels; j++)
		{
			aiNodeAnim* node = ani->mChannels[j];
			Animation oneAni;
			string AniName = node->mNodeName.C_Str();
			for (UINT j = 0; j < node->mNumPositionKeys; j++)
			{
				double time = node->mPositionKeys[j].mTime / ticks;
				XMFLOAT3 pos;
				SetVertexVector(pos, node->mPositionKeys[j].mValue);
				oneAni.T.push_back(make_pair(pos, time));
			}
			for (UINT j = 0; j < node->mNumRotationKeys; j++)
			{
				double time = node->mRotationKeys[j].mTime / ticks;
				XMFLOAT4 qua;
				SetVertexVector(qua, node->mRotationKeys[j].mValue);
				oneAni.R.push_back(make_pair(qua, time));
			}
			for (UINT j = 0; j < node->mNumScalingKeys; j++)
			{
				double time = node->mScalingKeys[j].mTime / ticks;
				XMFLOAT3 pos;
				SetVertexVector(pos, node->mScalingKeys[j].mValue);
				oneAni.S.push_back(make_pair(pos, time));
			}
			clip->clips.push_back(std::move(make_pair(AniName, oneAni)));
		}
		ResourceManager::AddAnimation(clip);
		asset->cliplist.push_back(clip);
	}
}

void ModelImport::ImportModel(string path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path,
		aiProcessPreset_TargetRealtime_Fast |
		aiProcess_ConvertToLeftHanded
	);

	if (!scene)
	{
		MessageBox(NULL, MakeWStr(importer.GetErrorString()).data(), TEXT("导入失败"), 0);
		return;
	}
	
	string rootname = FileUtility::GetFileName(path);
	asset.reset();
	asset = shared_ptr<Assets>(new Assets);
	asset->name = rootname;
	ProcessAnimation(scene);

	shared_ptr<GameObject> rootObject = GameObject::Instantiate(rootname);

	weak_ptr<Transform> myTrans = rootObject->GetTransform();
	aiVector3D aiS;
	aiVector3D aiT;
	aiQuaternion aiR;
	scene->mRootNode->mTransformation.Decompose(aiS, aiR, aiT);
	XMVECTOR vc;
	SetVertexVector(vc, aiT);
	myTrans.lock()->SetLocalPosition(vc);
	SetVertexVector(vc, aiS);
	myTrans.lock()->SetLocalScale(vc);
	SetQuater(vc, aiR);
	myTrans.lock()->SetLocalRotation(vc);

	for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
		ProcessNode(scene, rootObject, scene->mRootNode->mChildren[i]);

	for (auto& iter : setupAvatar)
	{
		iter.first.lock()->SetAvatar(iter.second);
	}
	setupAvatar.clear();
	ResourceManager::AddAssets(asset);
	MakePrefab(rootObject);
	MakeAssets(asset);
}

//FbxManager* FBXManager = NULL;
//FbxScene* FBXScene = NULL;
//
//#undef IOS_REF
//#define IOS_REF (*(FBXManager->GetIOSettings()))
//
//using namespace std;
//using namespace DirectX;
//using namespace EditorWindows;
//
//FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
//void ProcessNode(GameObject* model, FbxNode* node, FbxAnimLayer* layer);
//void ProcessMesh(GameObject* model, FbxNode* meshNode);
//void SetVertexColor(XMFLOAT4& color1, FbxColor& color2);
//void SetVertexUV(XMFLOAT2& UV1, FbxVector2& UV2);
//void SetVertexVector(XMFLOAT3& vector1, FbxVector4& vector2);
//
//void FBXImport::ImportModel(string path)
//{
//	FBXManager = FbxManager::Create();
//	FbxIOSettings* set = FbxIOSettings::Create(FBXManager, IOSROOT);
//	FBXManager->SetIOSettings(set);
//
//	IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
//	IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
//	IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
//	IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
//	IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
//	IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
//	IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
//
//	FBXScene = FbxScene::Create(FBXManager, "My Scene");
//
//	FbxImporter* importer = FbxImporter::Create(FBXManager, "");
//	int state = importer->GetStatus().GetCode();
//
//	if (!importer->Initialize(path.data(), -1, FBXManager->GetIOSettings()))
//	{
//		MessageBox(NULL, TEXT("导入失败了，可能是路径中含有中文"), TEXT("导入失败"), 0);
//		FBXScene->Destroy();
//		FBXManager->Destroy();
//		return;
//	}
//
//	importer->Import(FBXScene);
//	importer->Destroy();
//
//	//FbxAxisSystem::DirectX.ConvertScene(FBXScene);我发现这里SDK有BUG，正确的方式应该是用openGL坐标系，再反转Z轴坐标即可，动画那边也要注意。
//	FbxNode* root = FBXScene->GetRootNode();
//	int rootcount = root->GetChildCount();
//	for (int i = 0; i < rootcount; i++)
//	{
//		ProcessMesh(nullptr, root->GetChild(i));
//	}
//
//	int lAnimStackCount = importer->GetAnimStackCount();
//	LogWnd::Print(L"AnimStackCount   " +  to_wstring(lAnimStackCount));
//
//	FbxAnimLayer* layer = FBXScene->GetSrcObject<FbxAnimStack>(0)->GetMember<FbxAnimLayer>();
//	ProcessNode(nullptr, root->GetChild(0), layer);
//
//	FBXScene->Destroy();
//	FBXManager->Destroy();
//}
//
//FbxAMatrix GetGeometryTransformation(FbxNode* inNode)
//{
//	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
//	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
//	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);
//
//	return FbxAMatrix(lT, lR, lS);
//}
//
//void ProcessNode(GameObject* parent, FbxNode* node, FbxAnimLayer* layer)
//{
//	GameObject* mynode = new GameObject(node->GetName());
//	Transform* mytrans = mynode->GetTransform();
//	if (parent != nullptr)
//	{
//		mytrans->SetParent(parent->GetTransform());
//	}
//
//	FbxVector4 localS = node->EvaluateLocalScaling();
//	FbxVector4 localR = node->EvaluateLocalRotation();
//	FbxVector4 localT = node->EvaluateLocalTranslation();
//	XMVECTOR goodS;
//	XMVECTOR goodR;
//	XMVECTOR goodT;
//
//	XMMATRIX me;
//	me = XMMatrixRotationX(node->PreRotation.Get()[0] / 180 * XM_PI);
//	me *= XMMatrixRotationY(node->PreRotation.Get()[1] / 180 * XM_PI);
//	me *= XMMatrixRotationZ(node->PreRotation.Get()[2] / 180 * XM_PI);
//	me *= XMMatrixScaling(localS[0], localS[1], localS[2]);
//	me *= XMMatrixRotationX(localR[0] / 180 * XM_PI)*XMMatrixRotationY(localR[1] / 180 * XM_PI)*XMMatrixRotationZ(localR[2] / 180 * XM_PI);
//	me *= XMMatrixTranslation(localT[0], localT[1], localT[2]);
//	mytrans->SetLocalPosition(goodT);
//	mytrans->SetLocalRotation(goodR);
//	mytrans->SetLocalScale(goodS);
//	
//	Bone b;
//	b.name = node->GetName();
//	b.parentindex = parentindex;
//	Animation newAni;
//
//	b.preRotation[0] = node->PreRotation.Get()[0] / 180 * XM_PI;
//	b.preRotation[1] = node->PreRotation.Get()[1] / 180 * XM_PI;
//	b.preRotation[2] = node->PreRotation.Get()[2] / 180 * XM_PI;
//
//	FbxAnimCurve* curve = NULL;
//	int lKeyCount;
//	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.T[0].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i) ,curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.T[0].push_back(key);
//		}
//	}
//	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.T[1].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i) ,curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.T[1].push_back(key);
//		}
//	}
//	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.T[2].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i) ,curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.T[2].push_back(key);
//		}
//	}
//	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.R[0].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i) / 180 * XM_PI ,curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.R[0].push_back(key);
//		}
//	}
//	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.R[1].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i) / 180 * XM_PI ,curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.R[1].push_back(key);
//		}
//	}
//	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.R[2].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i) / 180 * XM_PI ,curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.R[2].push_back(key);
//		}
//	}
//	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.S[0].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i),curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.S[0].push_back(key);
//		}
//	}
//	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.S[1].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i),curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.S[1].push_back(key);
//		}
//	}
//	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
//	if (curve)
//	{
//		lKeyCount = curve->KeyGetCount();
//		newAni.S[2].reserve(lKeyCount);
//		for (int i = 0; i < lKeyCount; i++)
//		{
//			array<float, 2> key = { curve->KeyGetValue(i),curve->KeyGetTime(i).GetSecondDouble() };
//			newAni.S[2].push_back(key);
//		}
//	}
//	int index = model->bones.size();
//	b.Animaions.push_back(std::move(newAni));
//	model->bones.push_back(std::move(b));
//
//	for (int i = 0; i < node->GetChildCount(); i++)
//	{
//		ProcessBones(model, node->GetChild(i), layer, index);
//	}
//}
//
//void ProcessMesh(GameObject* parent, FbxNode* meshNode)
//{
//	GameObject* mynode = new GameObject(meshNode->GetName());
//	Transform* mytrans = mynode->GetTransform();
//	if (parent != nullptr)
//	{
//		mytrans->SetParent(parent->GetTransform());
//	}
//	
//	FbxVector4 localS = meshNode->EvaluateLocalScaling();
//	FbxVector4 localR = meshNode->EvaluateLocalRotation();
//	FbxVector4 localT = meshNode->EvaluateLocalTranslation();
//	XMVECTOR goodS;
//	XMVECTOR goodR;
//	XMVECTOR goodT;
//
//	XMMATRIX me;
//	me = XMMatrixRotationX(meshNode->PreRotation.Get()[0] / 180 * XM_PI);
//	me *= XMMatrixRotationY(meshNode->PreRotation.Get()[1] / 180 * XM_PI);
//	me *= XMMatrixRotationZ(meshNode->PreRotation.Get()[2] / 180 * XM_PI);
//	me *= XMMatrixScaling(localS[0], localS[1], localS[2]);
//	me *= XMMatrixRotationX(localR[0] / 180 * XM_PI)*XMMatrixRotationY(localR[1] / 180 * XM_PI)*XMMatrixRotationZ(localR[2] / 180 * XM_PI);
//	me *= XMMatrixTranslation(localT[0], localT[1], localT[2]);
//	ASSERT(XMMatrixDecompose(&goodS, &goodR, &goodT, me), "草拟吗FBX SDK");
//	mytrans->SetLocalPosition(goodT);
//	mytrans->SetLocalRotation(goodR);
//	mytrans->SetLocalScale(goodS);
//	
//	if (meshNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh&&!ResourceManager::checkifExit(meshNode->GetName()))
//	{
//		FbxMesh* mesh = static_cast<FbxMesh*>(meshNode->GetNodeAttribute());
//		UINT SkinCount = mesh->GetDeformerCount();
//		
//		if (SkinCount > 0)
//		{
//			AniMesh meshdata;
//			meshdata.name = meshNode->GetName();
//			int vertexcount = mesh->GetControlPointsCount();
//			FbxVector4* lControlPoints = mesh->GetControlPoints();
//			std::vector<AniVertex> V(vertexcount);
//			FbxAMatrix lGlobal = GetGeometryTransformation(meshNode);
//
//			for (int i = 0; i < vertexcount; ++i)
//			{
//				FbxVector4 pos = lGlobal.MultT(lControlPoints[i]);
//				V[i].pos.x = pos[0];
//				V[i].pos.y = pos[1];
//				V[i].pos.z = pos[2];//这里一定要反转Z轴
//
//				for (int j = 0; j < mesh->GetElementNormalCount() && j < 1; j++)
//				{
//					FbxGeometryElementNormal* leNormals = mesh->GetElementNormal(j);
//					if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//					{
//						if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
//							SetVertexVector(meshdata.vertexs[i].normal, leNormals->GetDirectArray().GetAt(i));
//					}
//				}
//
//			}
//			meshdata.vertexs = std::move(V);
//
//			int polycount = mesh->GetPolygonCount();
//			std::vector<std::vector<UINT>> indices;
//
//			bool lIsAllSame = true;
//			for (int i = 0; i < mesh->GetElementMaterialCount(); i++)
//			{
//				FbxGeometryElementMaterial* lMaterialElement = mesh->GetElementMaterial(i);
//				if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
//				{
//					lIsAllSame = false;
//					break;
//				}
//			}
//			if (lIsAllSame)
//			{
//				std::vector<UINT> I(polycount * 3);
//				for (int i = 0; i < polycount; i++)
//				{
//					for (int j = 0; j < 3; j++)
//					{
//						int lControlPointIndex = mesh->GetPolygonVertex(i, j);
//						I[i * 3 + j] = lControlPointIndex;
//					}
//				}
//				indices.push_back(std::move(I));
//			}
//			else
//			{
//				std::vector<int> groupID;
//				for (int i = 0; i < polycount; i++)
//				{
//					FbxGeometryElementMaterial* lMaterialElement = mesh->GetElementMaterial(0);
//					int lMatId = lMaterialElement->GetIndexArray().GetAt(i);
//					int j = 0;
//					for (; j < groupID.size(); j++)
//					{
//						if (groupID[j] == lMatId)
//						{
//							for (int k = 0; k < 3; k++)
//							{
//								int lControlPointIndex = mesh->GetPolygonVertex(i, k);
//								indices[j].push_back(lControlPointIndex);
//							}
//						}
//						break;
//					}
//					if (j == groupID.size())
//					{
//						std::vector<UINT> I(3);
//						for (int k = 0; k < 3; k++)
//						{
//							int lControlPointIndex = mesh->GetPolygonVertex(i, k);
//							I[k] = lControlPointIndex;
//						}
//						indices.push_back(I);
//						groupID.push_back(lMatId);
//					}
//				}
//			}
//			int vertexId = 0;
//			for (int i = 0; i < polycount; i++)
//			{
//				for (int j = 0; j < 3; j++)
//				{
//					int lControlPointIndex = mesh->GetPolygonVertex(i, j);
//					for (int l = 0; l < mesh->GetElementVertexColorCount() && l < 1; l++)
//					{
//						FbxGeometryElementVertexColor* leVtxc = mesh->GetElementVertexColor(l);
//						switch (leVtxc->GetMappingMode())
//						{
//						default:
//							break;
//						case FbxGeometryElement::eByControlPoint:
//							switch (leVtxc->GetReferenceMode())
//							{
//							case FbxGeometryElement::eDirect:
//								SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
//								break;
//							case FbxGeometryElement::eIndexToDirect:
//							{
//								int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
//								SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(id));
//							}
//							break;
//							default:
//								break; // other reference modes not shown here!
//							}
//							break;
//
//						case FbxGeometryElement::eByPolygonVertex:
//						{
//							switch (leVtxc->GetReferenceMode())
//							{
//							case FbxGeometryElement::eDirect:
//								SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(vertexId));
//								break;
//							case FbxGeometryElement::eIndexToDirect:
//							{
//								int id = leVtxc->GetIndexArray().GetAt(vertexId);
//								SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(id));
//							}
//							break;
//							default:
//								break; // other reference modes not shown here!
//							}
//						}
//						break;
//
//						case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
//						case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
//						case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
//							break;
//						}
//					}
//					for (int l = 0; l < mesh->GetElementUVCount() && l < 2; ++l)
//					{
//						FbxGeometryElementUV* leUV = mesh->GetElementUV(l);
//
//						XMFLOAT2& m_uv = l == 0 ? meshdata.vertexs[lControlPointIndex].UV0 : meshdata.vertexs[lControlPointIndex].UV1;
//						switch (leUV->GetMappingMode())
//						{
//						default:
//							break;
//						case FbxGeometryElement::eByControlPoint:
//							switch (leUV->GetReferenceMode())
//							{
//							case FbxGeometryElement::eDirect:
//								SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(lControlPointIndex));
//								break;
//							case FbxGeometryElement::eIndexToDirect:
//							{
//								int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
//								SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(id));
//							}
//							break;
//							default:
//								break; // other reference modes not shown here!
//							}
//							break;
//
//						case FbxGeometryElement::eByPolygonVertex:
//						{
//							int lTextureUVIndex = mesh->GetTextureUVIndex(i, j);
//							switch (leUV->GetReferenceMode())
//							{
//							case FbxGeometryElement::eDirect:
//							case FbxGeometryElement::eIndexToDirect:
//							{
//								SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(lTextureUVIndex));
//							}
//							break;
//							default:
//								break; // other reference modes not shown here!
//							}
//						}
//						break;
//
//						case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
//						case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
//						case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
//							break;
//						}
//					}
//					for (int l = 0; l < mesh->GetElementNormalCount() && l < 1; ++l)
//					{
//						FbxGeometryElementNormal* leNormal = mesh->GetElementNormal(l);
//
//						if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//						{
//							switch (leNormal->GetReferenceMode())
//							{
//							case FbxGeometryElement::eDirect:
//								SetVertexVector(meshdata.vertexs[lControlPointIndex].normal, leNormal->GetDirectArray().GetAt(vertexId));
//								break;
//							case FbxGeometryElement::eIndexToDirect:
//							{
//								int id = leNormal->GetIndexArray().GetAt(vertexId);
//								SetVertexVector(meshdata.vertexs[lControlPointIndex].normal, leNormal->GetDirectArray().GetAt(id));
//							}
//							break;
//							default:
//								break; // other reference modes not shown here!
//							}
//						}
//
//					}
//					for (int l = 0; l < mesh->GetElementTangentCount() && l < 1; ++l)
//					{
//						FbxGeometryElementTangent* leTangent = mesh->GetElementTangent(l);
//
//						if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//						{
//							switch (leTangent->GetReferenceMode())
//							{
//							case FbxGeometryElement::eDirect:
//								SetVertexVector(meshdata.vertexs[lControlPointIndex].tangent, leTangent->GetDirectArray().GetAt(vertexId));
//								break;
//							case FbxGeometryElement::eIndexToDirect:
//							{
//								int id = leTangent->GetIndexArray().GetAt(vertexId);
//								SetVertexVector(meshdata.vertexs[lControlPointIndex].tangent, leTangent->GetDirectArray().GetAt(id));
//							}
//							break;
//							default:
//								break; // other reference modes not shown here!
//							}
//						}
//					}
//					vertexId++;
//				}
//			}
//			meshdata.indexs = std::move(indices);
//			Avatar avatar;
//
//			FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin); //只取第一套蒙皮
//			UINT bonecount = skin->GetClusterCount();
//			for (int j = 0; j < bonecount; j++)
//			{
//				FbxCluster* bone = skin->GetCluster(j);
//				FbxAMatrix meshM;
//				FbxAMatrix linkM;
//				bone->GetTransformMatrix(meshM);
//				bone->GetTransformLinkMatrix(linkM);
//				linkM = linkM.Inverse()*meshM*GetGeometryTransformation(meshNode);
//
//				string n = bone->GetLink()->GetName();
//				for (int k = 0; k < model->bones.size(); k++)
//				{
//					if (model->bones[k].name == n)
//					{
//						for (int b = 0; b < 4; b++)
//						{
//							for (int a = 0; a < 4; a++)
//							{
//								model->bones[k].Bind.m[b][a] = linkM.Get(b, a);
//							}
//						}
//
//						int c = bone->GetControlPointIndicesCount();
//						for (int l = 0; l < c; l++)
//						{
//							meshdata.vertexs[bone->GetControlPointIndices()[l]].BoneWeights.push_back(bone->GetControlPointWeights()[l]);
//							meshdata.vertexs[bone->GetControlPointIndices()[l]].BoneIndex.push_back(k);
//						}
//					}
//				}
//			}
//			ResourceManager::AddMesh(meshdata);
//		}
//		else
//		{
//
//		}
//
//		int vertexcount = mesh->GetControlPointsCount();
//		FbxVector4* lControlPoints = mesh->GetControlPoints();
//		std::vector<Vertex> V(vertexcount);
//
//		for (int i = 0; i < vertexcount; ++i)
//		{ 
//			FbxVector4 pos = lGlobal.MultT(lControlPoints[i]);
//			V[i].pos.x = pos[0];
//			V[i].pos.y = pos[1];
//			V[i].pos.z = pos[2];//这里一定要反转Z轴
//			
//			for (int j = 0; j < mesh->GetElementNormalCount()&&j<1; j++)
//			{
//				FbxGeometryElementNormal* leNormals = mesh->GetElementNormal(j);
//				if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//				{		
//					if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
//						SetVertexVector(meshdata.vertexs[i].normal, leNormals->GetDirectArray().GetAt(i));
//				}
//			}
//
//		}
//		meshdata.vertexs = std::move(V);
//
//		int polycount = mesh->GetPolygonCount();
//		std::vector<UINT> I(polycount * 3);	
//		int vertexId = 0;
//		for (int i = 0; i < polycount; i++)
//		{
//
//			int lPolygonSize = mesh->GetPolygonSize(i);
//
//			for (int j = 0; j < lPolygonSize; j++)
//			{
//				int lControlPointIndex = mesh->GetPolygonVertex(i, j);
//				I[i * 3 + j] = lControlPointIndex;
//
//				for (int l = 0; l < mesh->GetElementVertexColorCount() && l < 1; l++)
//				{
//					FbxGeometryElementVertexColor* leVtxc = mesh->GetElementVertexColor(l);
//					switch (leVtxc->GetMappingMode())
//					{
//					default:
//						break;
//					case FbxGeometryElement::eByControlPoint:
//						switch (leVtxc->GetReferenceMode())
//						{
//						case FbxGeometryElement::eDirect:
//							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
//							break;
//						case FbxGeometryElement::eIndexToDirect:
//						{
//							int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
//							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(id));
//						}
//						break;
//						default:
//							break; // other reference modes not shown here!
//						}
//						break;
//
//					case FbxGeometryElement::eByPolygonVertex:
//					{
//						switch (leVtxc->GetReferenceMode())
//						{
//						case FbxGeometryElement::eDirect:
//							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(vertexId));
//							break;
//						case FbxGeometryElement::eIndexToDirect:
//						{
//							int id = leVtxc->GetIndexArray().GetAt(vertexId);
//							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(id));
//						}
//						break;
//						default:
//							break; // other reference modes not shown here!
//						}
//					}
//					break;
//
//					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
//					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
//					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
//						break;
//					}
//				}
//				for (int l = 0; l < mesh->GetElementUVCount() && l<2; ++l)
//				{
//					FbxGeometryElementUV* leUV = mesh->GetElementUV(l);
//								
//				    XMFLOAT2& m_uv = l==0?meshdata.vertexs[lControlPointIndex].UV0:meshdata.vertexs[lControlPointIndex].UV1;
//					switch (leUV->GetMappingMode())
//					{
//					default:
//						break;
//					case FbxGeometryElement::eByControlPoint:
//						switch (leUV->GetReferenceMode())
//						{
//						case FbxGeometryElement::eDirect:
//							SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(lControlPointIndex));
//							break;
//						case FbxGeometryElement::eIndexToDirect:
//						{
//							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
//							SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(id));
//						}
//						break;
//						default:
//							break; // other reference modes not shown here!
//						}
//						break;
//
//					case FbxGeometryElement::eByPolygonVertex:
//					{
//						int lTextureUVIndex = mesh->GetTextureUVIndex(i, j);
//						switch (leUV->GetReferenceMode())
//						{
//						case FbxGeometryElement::eDirect:
//						case FbxGeometryElement::eIndexToDirect:
//						{
//							SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(lTextureUVIndex));
//						}
//						break;
//						default:
//							break; // other reference modes not shown here!
//						}
//					}
//					break;
//
//					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
//					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
//					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
//						break;
//					}
//				}
//				for (int l = 0; l < mesh->GetElementNormalCount() && l<1; ++l)
//				{
//					FbxGeometryElementNormal* leNormal = mesh->GetElementNormal(l);
//			
//					if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//					{
//						switch (leNormal->GetReferenceMode())
//						{
//						case FbxGeometryElement::eDirect:
//							SetVertexVector(meshdata.vertexs[lControlPointIndex].normal, leNormal->GetDirectArray().GetAt(vertexId));
//							break;
//						case FbxGeometryElement::eIndexToDirect:
//						{
//							int id = leNormal->GetIndexArray().GetAt(vertexId);
//							SetVertexVector(meshdata.vertexs[lControlPointIndex].normal, leNormal->GetDirectArray().GetAt(id));
//						}
//						break;
//						default:
//							break; // other reference modes not shown here!
//						}
//					}
//
//				}
//				for (int l = 0; l < mesh->GetElementTangentCount() && l<1; ++l)
//				{
//					FbxGeometryElementTangent* leTangent = mesh->GetElementTangent(l);
//
//					if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//					{
//						switch (leTangent->GetReferenceMode())
//						{
//						case FbxGeometryElement::eDirect:
//							SetVertexVector(meshdata.vertexs[lControlPointIndex].tangent, leTangent->GetDirectArray().GetAt(vertexId));
//							break;
//						case FbxGeometryElement::eIndexToDirect:
//						{
//							int id = leTangent->GetIndexArray().GetAt(vertexId);
//							SetVertexVector(meshdata.vertexs[lControlPointIndex].tangent, leTangent->GetDirectArray().GetAt(id));
//						}
//						break;
//						default:
//							break; // other reference modes not shown here!
//						}
//					}
//				}
//				vertexId++;
//			} 
//		} 
//		meshdata.indexs = std::move(I);
//
//		UINT SkinCount = mesh->GetDeformerCount();
//		for (int i = 0; i < SkinCount; i++)
//		{
//			FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(i, FbxDeformer::eSkin);
//			UINT bonecount = skin->GetClusterCount();
//			for (int j = 0; j < bonecount; j++)
//			{
//				FbxCluster* bone = skin->GetCluster(j);
//				FbxAMatrix meshM;
//				FbxAMatrix linkM;
//				bone->GetTransformMatrix(meshM);
//				bone->GetTransformLinkMatrix(linkM);
//				linkM = linkM.Inverse()*meshM*GetGeometryTransformation(meshNode);
//
//				string n = bone->GetLink()->GetName();
//				for (int k = 0; k < model->bones.size(); k++)
//				{
//					if (model->bones[k].name == n)
//					{
//						for (int b = 0; b < 4; b++)
//						{
//							for (int a = 0; a < 4; a++)
//							{
//								model->bones[k].Bind.m[b][a] = linkM.Get(b, a);
//							}
//						}
//
//						int c = bone->GetControlPointIndicesCount();
//						for (int l = 0; l < c; l++)
//						{
//							meshdata.vertexs[bone->GetControlPointIndices()[l]].BoneWeights.push_back(bone->GetControlPointWeights()[l]);
//							meshdata.vertexs[bone->GetControlPointIndices()[l]].BoneIndex.push_back(k);
//						}
//					}
//				}
//			}
//		}
//
//		meshdata.parentindex = parentindex;
//		model->meshs.push_back(std::move(meshdata));
//	}
//
//	for (int i = 0; i < meshNode->GetChildCount(); i++)
//	{
//		ProcessMesh(model, meshNode->GetChild(i), index);
//	}
//}
//
//void SetVertexColor(XMFLOAT4& color1, FbxColor& color2)
//{
//	color1.x = color2.mRed;
//	color1.y = color2.mGreen;
//	color1.z = color2.mBlue;
//	color1.w = color2.mAlpha;
//}
//
//void SetVertexUV(XMFLOAT2& UV1,FbxVector2& UV2)
//{
//	UV1.x = UV2[0];
//	UV1.y = UV2[1];
//}
//
//void SetVertexVector(XMFLOAT3& vector1, FbxVector4& vector2)
//{
//	vector1.x = vector2[0];
//	vector1.y = vector2[1];
//	vector1.z = vector2[2];
//}