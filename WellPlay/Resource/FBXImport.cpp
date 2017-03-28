#include "stdafx.h"
#include "FBXImport.h"
#include <fbxsdk.h>

FbxManager* FBXManager = NULL;
FbxScene* FBXScene = NULL;

#undef IOS_REF
#define IOS_REF (*(FBXManager->GetIOSettings()))

using namespace std;
using namespace DirectX;

FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
void ProcessBones(Model* model, FbxNode* node, FbxAnimLayer* layer, int parentindex);
void ProcessMesh(Model* model, FbxNode* meshNode, int parentindex);
void SetVertexColor(XMFLOAT4& color1, FbxColor& color2);
void SetVertexUV(XMFLOAT2& UV1, FbxVector2& UV2);
void SetVertexVector(XMFLOAT3& vector1, FbxVector4& vector2);

shared_ptr<Model> FBXImport::ImportModel(string path)
{
	FBXManager = FbxManager::Create();
	FbxIOSettings* set = FbxIOSettings::Create(FBXManager, IOSROOT);
	FBXManager->SetIOSettings(set);

	IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
	IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
	IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
	IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
	IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
	IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
	IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	FBXScene = FbxScene::Create(FBXManager, "My Scene");

	FbxImporter* importer = FbxImporter::Create(FBXManager, "");
	int state = importer->GetStatus().GetCode();

	if (!importer->Initialize(path.data(), -1, FBXManager->GetIOSettings()))
	{
		MessageBox(NULL, TEXT("导入失败了，可能是路径中含有中文!"), TEXT("导入失败"), 0);
		FBXScene->Destroy();
		FBXManager->Destroy();

		return nullptr;
	}

	importer->Import(FBXScene);
	importer->Destroy();

	//FbxAxisSystem::DirectX.ConvertScene(FBXScene);我发现这里SDK有BUG，正确的方式应该是用openGL坐标系，再反转Z轴坐标即可，动画那边也要注意。

	FbxNode* root = FBXScene->GetRootNode();
	int rootcount = root->GetChildCount();
	Model* model = new Model();

	FbxAnimLayer* layer = FBXScene->GetSrcObject<FbxAnimStack>(0)->GetMember<FbxAnimLayer>();
	ProcessBones(model, root->GetChild(0), layer, -1);

	for (int i = 0; i < rootcount; i++)
	{
		ProcessMesh(model, root->GetChild(i), -1);
	}
	FBXScene->Destroy();
	FBXManager->Destroy();

	return shared_ptr<Model>(model);
}

FbxAMatrix GetGeometryTransformation(FbxNode* inNode)
{
	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

void ProcessBones(Model* model, FbxNode* node, FbxAnimLayer* layer, int parentindex)
{
	Bone b;
	b.name = node->GetName();
	b.parentindex = parentindex;
	Animation newAni;

	b.preRotation[0] = node->PreRotation.Get()[0] / 180 * XM_PI;
	b.preRotation[1] = node->PreRotation.Get()[1] / 180 * XM_PI;
	b.preRotation[2] = node->PreRotation.Get()[2] / 180 * XM_PI;

	FbxAnimCurve* curve = NULL;
	int lKeyCount;
	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.T[0].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i) ,curve->KeyGetTime(i).GetSecondDouble() };
			newAni.T[0].push_back(key);
		}
	}
	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.T[1].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i) ,curve->KeyGetTime(i).GetSecondDouble() };
			newAni.T[1].push_back(key);
		}
	}
	curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.T[2].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i) ,curve->KeyGetTime(i).GetSecondDouble() };
			newAni.T[2].push_back(key);
		}
	}
	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.R[0].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i) / 180 * XM_PI ,curve->KeyGetTime(i).GetSecondDouble() };
			newAni.R[0].push_back(key);
		}
	}
	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.R[1].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i) / 180 * XM_PI ,curve->KeyGetTime(i).GetSecondDouble() };
			newAni.R[1].push_back(key);
		}
	}
	curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.R[2].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i) / 180 * XM_PI ,curve->KeyGetTime(i).GetSecondDouble() };
			newAni.R[2].push_back(key);
		}
	}
	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.S[0].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i),curve->KeyGetTime(i).GetSecondDouble() };
			newAni.S[0].push_back(key);
		}
	}
	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.S[1].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i),curve->KeyGetTime(i).GetSecondDouble() };
			newAni.S[1].push_back(key);
		}
	}
	curve = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve)
	{
		lKeyCount = curve->KeyGetCount();
		newAni.S[2].reserve(lKeyCount);
		for (int i = 0; i < lKeyCount; i++)
		{
			array<float, 2> key = { curve->KeyGetValue(i),curve->KeyGetTime(i).GetSecondDouble() };
			newAni.S[2].push_back(key);
		}
	}
	int index = model->bones.size();
	b.Animaions.push_back(std::move(newAni));
	model->bones.push_back(std::move(b));

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessBones(model, node->GetChild(i), layer, index);
	}
}

void ProcessMesh(Model* model, FbxNode* meshNode, int parentindex)
{
	FbxAMatrix lGlobal;
	lGlobal = meshNode->EvaluateGlobalTransform()*GetGeometryTransformation(meshNode);
	int index = model->meshs.size();

	if (meshNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* mesh = static_cast<FbxMesh*>(meshNode->GetNodeAttribute());

		Mesh meshdata;
		int vertexcount = mesh->GetControlPointsCount();
		FbxVector4* lControlPoints = mesh->GetControlPoints();
		std::vector<Vertex> V(vertexcount);

		for (int i = 0; i < vertexcount; ++i)
		{ 
			//FbxVector4 pos = lGlobal.MultT(lControlPoints[i]);
			FbxVector4 pos = lControlPoints[i];
			V[i].pos.x = pos[0];
			V[i].pos.y = pos[1];
			V[i].pos.z = pos[2];//这里一定要反转Z轴
			
			for (int j = 0; j < mesh->GetElementNormalCount()&&j<1; j++)
			{
				FbxGeometryElementNormal* leNormals = mesh->GetElementNormal(j);
				if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{		
					if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
						SetVertexVector(meshdata.vertexs[i].normal, leNormals->GetDirectArray().GetAt(i));
				}
			}

		}
		//int Ioffset = VCount;
		//VCount += vertexcount;
		meshdata.vertexs = std::move(V);

		int polycount = mesh->GetPolygonCount();
		std::vector<UINT> I(polycount * 3);	
		int vertexId = 0;
		for (int i = 0; i < polycount; i++)
		{
			for (int l = 0; l < mesh->GetElementPolygonGroupCount(); l++)
			{
				FbxGeometryElementPolygonGroup* lePolgrp = mesh->GetElementPolygonGroup(l);
				switch (lePolgrp->GetMappingMode())
				{
				case FbxGeometryElement::eByPolygon:
					if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
					{
						int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
						//DisplayInt(header, polyGroupId);
						break;
					}
				default:
					// any other mapping modes don't make sense
					//DisplayString("        \"unsupported group assignment\"");
					break;
				}
			}

			int lPolygonSize = mesh->GetPolygonSize(i);

			for (int j = 0; j < lPolygonSize; j++)
			{
				int lControlPointIndex = mesh->GetPolygonVertex(i, j);
				I[i * 3 + j] = lControlPointIndex;

				for (int l = 0; l < mesh->GetElementVertexColorCount() && l < 1; l++)
				{
					FbxGeometryElementVertexColor* leVtxc = mesh->GetElementVertexColor(l);
					switch (leVtxc->GetMappingMode())
					{
					default:
						break;
					case FbxGeometryElement::eByControlPoint:
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
					{
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leVtxc->GetIndexArray().GetAt(vertexId);
							SetVertexColor(meshdata.vertexs[lControlPointIndex].color, leVtxc->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}
				for (int l = 0; l < mesh->GetElementUVCount() && l<2; ++l)
				{
					FbxGeometryElementUV* leUV = mesh->GetElementUV(l);
								
				    XMFLOAT2& m_uv = l==0?meshdata.vertexs[lControlPointIndex].UV0:meshdata.vertexs[lControlPointIndex].UV1;
					switch (leUV->GetMappingMode())
					{
					default:
						break;
					case FbxGeometryElement::eByControlPoint:
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
							SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = mesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
						{
							SetVertexUV(m_uv, leUV->GetDirectArray().GetAt(lTextureUVIndex));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}
				for (int l = 0; l < mesh->GetElementNormalCount() && l<1; ++l)
				{
					FbxGeometryElementNormal* leNormal = mesh->GetElementNormal(l);
			
					if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							SetVertexVector(meshdata.vertexs[lControlPointIndex].normal, leNormal->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							SetVertexVector(meshdata.vertexs[lControlPointIndex].normal, leNormal->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}

				}
				for (int l = 0; l < mesh->GetElementTangentCount() && l<1; ++l)
				{
					FbxGeometryElementTangent* leTangent = mesh->GetElementTangent(l);

					if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leTangent->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							SetVertexVector(meshdata.vertexs[lControlPointIndex].tangent, leTangent->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leTangent->GetIndexArray().GetAt(vertexId);
							SetVertexVector(meshdata.vertexs[lControlPointIndex].tangent, leTangent->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}
				}
				vertexId++;
			} 
		} 
		//ICount += indexcount;
		meshdata.indexs = std::move(I);

		UINT SkinCount = mesh->GetDeformerCount();
		for (int i = 0; i < SkinCount; i++)
		{
			FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(i, FbxDeformer::eSkin);
			UINT bonecount = skin->GetClusterCount();
			for (int j = 0; j < bonecount; j++)
			{
				FbxCluster* bone = skin->GetCluster(j);
				FbxAMatrix meshM;
				FbxAMatrix linkM;
				bone->GetTransformMatrix(meshM);
				bone->GetTransformLinkMatrix(linkM);
				linkM = linkM.Inverse()*meshM*GetGeometryTransformation(meshNode);

				string n = bone->GetLink()->GetName();
				for (int k = 0; k < model->bones.size(); k++)
				{
					if (model->bones[k].name == n)
					{
						for (int b = 0; b < 4; b++)
						{
							for (int a = 0; a < 4; a++)
							{
								model->bones[k].Bind.m[b][a] = linkM.Get(b, a);
							}
						}

						int c = bone->GetControlPointIndicesCount();
						for (int l = 0; l < c; l++)
						{
							meshdata.vertexs[bone->GetControlPointIndices()[l]].BoneWeights.push_back(bone->GetControlPointWeights()[l]);
							meshdata.vertexs[bone->GetControlPointIndices()[l]].BoneIndex.push_back(k);
						}
					}
				}
			}
		}

		meshdata.parentindex = parentindex;
		model->meshs.push_back(std::move(meshdata));
	}

	for (int i = 0; i < meshNode->GetChildCount(); i++)
	{
		ProcessMesh(model, meshNode->GetChild(i), index);
	}
}

void SetVertexColor(XMFLOAT4& color1, FbxColor& color2)
{
	color1.x = color2.mRed;
	color1.y = color2.mGreen;
	color1.z = color2.mBlue;
	color1.w = color2.mAlpha;
}

void SetVertexUV(XMFLOAT2& UV1,FbxVector2& UV2)
{
	UV1.x = UV2[0];
	UV1.y = UV2[1];
}

void SetVertexVector(XMFLOAT3& vector1, FbxVector4& vector2)
{
	vector1.x = vector2[0];
	vector1.y = vector2[1];
	vector1.z = vector2[2];
}

//void Ani()
//{
//	for (int i = 0; i < model->bones.size(); i++)
//	{
//		XMMATRIX parent;
//		vector<Bone>& BoneList = model->bones;
//		if (BoneList[i].parentindex == -1)
//			parent = XMMatrixIdentity();
//		else
//			parent = XMLoadFloat4x4(&BoneList[BoneList[i].parentindex].SRT);
//		XMMATRIX me;
//		me = XMMatrixRotationX(BoneList[i].preRotation[0]);
//		me *= XMMatrixRotationY(BoneList[i].preRotation[1]);
//		me *= XMMatrixRotationZ(BoneList[i].preRotation[2]);
//		me *= XMMatrixScaling(BoneList[i].S[0], BoneList[i].S[1], BoneList[i].S[2]);
//		me *= XMMatrixRotationX(BoneList[i].R[0])*XMMatrixRotationY(BoneList[i].R[1])*XMMatrixRotationZ(BoneList[i].R[2]);
//		me *= XMMatrixTranslation(BoneList[i].T[0], BoneList[i].T[1], BoneList[i].T[2]);
//
//		XMStoreFloat4x4(&BoneList[i].SRT, me*parent);
//
//	}
//
//	for (int i = 0; i < cubeVertices.size(); i++)
//	{
//		VertexPositionColor Vertices = { XMFLOAT3(0,0,0),XMFLOAT3(0,0,0) };
//		for (int j = 0; j < Boneindex[i].size(); j++)
//		{
//			int index = Boneindex[i][j];
//			/*cubeVertices[i].pos =*/
//			XMVECTOR p = g_XMIdentityR3;
//			p.m128_f32[0] = cubeVertices[i].pos.x;
//			p.m128_f32[1] = cubeVertices[i].pos.y;
//			p.m128_f32[2] = cubeVertices[i].pos.z;
//
//			p = XMVector4Transform(p, XMLoadFloat4x4(&BoneList[index].Bind)*XMLoadFloat4x4(&BoneList[index].SRT));
//			Vertices.pos.x += p.m128_f32[0] * weights[i][j];
//			Vertices.pos.y += p.m128_f32[1] * weights[i][j];
//			Vertices.pos.z -= p.m128_f32[2] * weights[i][j];
//		}
//		cubeVertices[i] = Vertices;
//	}
//
//}