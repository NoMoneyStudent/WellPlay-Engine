#include "stdafx.h"
#include "Serialize.h"
#include <cereal\archives\json.hpp>
#include <cereal\archives\binary.hpp>
#include <cereal\types\polymorphic.hpp>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\types\string.hpp>
#include <cereal\types\array.hpp>
#include <cereal\types\tuple.hpp>
#include <cereal\types\utility.hpp>
#include <fstream>

#include "EngineRuntime\GameObject.h"
#include "EngineRuntime\Transform.h"
#include "EngineRuntime\SkinMeshRender.h"
#include "EngineRuntime\MeshRender.h"
#include "EngineRuntime\Render.h"
#include "EngineRuntime\Animator.h"
#include "EngineRuntime\EditorComponent.h"

#include "Model.h"
#include "ResourceManager.h"

CEREAL_REGISTER_TYPE(Transform);
CEREAL_REGISTER_TYPE(SkinMeshRender);
CEREAL_REGISTER_TYPE(MeshRender);
CEREAL_REGISTER_TYPE(Render);
CEREAL_REGISTER_TYPE(Animator);
CEREAL_REGISTER_TYPE(EditorComponent);

CEREAL_REGISTER_TYPE(CommonMesh);
CEREAL_REGISTER_TYPE(AniMesh);

CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Transform)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, EditorComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Animator)
CEREAL_REGISTER_POLYMORPHIC_RELATION(EditorComponent, Render)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Render, SkinMeshRender)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Render, MeshRender)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Mesh, CommonMesh)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Mesh, AniMesh)

using namespace std;
using namespace DirectX;

#pragma region 序列化模板函数
namespace cereal
{
	template<class Archive>
	void serialize(Archive & archive, XMFLOAT2 & vector)
	{
		archive(vector.x, vector.y);
	}

	template<class Archive>
	void serialize(Archive & archive, XMFLOAT3 & vector)
	{
		archive(vector.x, vector.y, vector.z);
	}

	template<class Archive>
	void serialize(Archive & archive, XMFLOAT4 & vector)
	{
		archive(vector.x, vector.y, vector.z,vector.w);
	}

	template<class Archive>
	void serialize(Archive & archive, XMFLOAT4X4 & matrix)
	{
		archive(matrix._11, matrix._12, matrix._13, matrix._14, 
			matrix._21, matrix._22, matrix._23, matrix._24, 
			matrix._31, matrix._32, matrix._33, matrix._34, 
			matrix._41, matrix._42, matrix._43, matrix._44);
	}

	template<class Archive>
	void serialize(Archive & archive, AniMesh & mesh)
	{
		archive(mesh.vertexs, cereal::base_class<Mesh>(&mesh));
	}
	template<class Archive>
	void serialize(Archive & archive, CommonMesh & mesh)
	{
		archive(mesh.vertexs, cereal::base_class<Mesh>(&mesh));
	}
	template<class Archive>
	void serialize(Archive & archive, Mesh & mesh)
	{
		archive(mesh.name,mesh.indexs);
	}
	template<class Archive>
	void serialize(Archive & archive, AniVertex & vertex)
	{
		archive(vertex.BoneWeights,vertex.BoneIndex, cereal::base_class<CommonVertex>(&vertex));
	}
	template<class Archive>
	void serialize(Archive & archive, CommonVertex & vertex)
	{
		archive(vertex.pos,vertex.color,vertex.normal,vertex.tangent,vertex.UV0,vertex.UV1);
	}

	template<class Archive>
	void serialize(Archive & archive, AnimationClip & animationclip)
	{
		archive(animationclip.name, animationclip.durning, animationclip.loop);
		archive(animationclip.clips);
	}
	template<class Archive>
	void serialize(Archive & archive, Animation & clip)
	{
		archive(clip.T,clip.R,clip.S);
	}

	template<class Archive>
	void serialize(Archive & archive, Avatar & avatar)
	{
		archive(avatar.name,avatar.bonelists);
	}
	template<class Archive>
	void serialize(Archive & archive, Bone & bone)
	{
		archive(bone.Bind,bone.name);
	}

	template<class Archive>
	void serialize(Archive & archive, Assets & asset)
	{
		archive(asset.name,asset.meshlist,asset.avatarlist,asset.cliplist);
	}
}
#pragma endregion

void MakeAssets(shared_ptr<Assets> asset)
{
	{
		//std::ofstream os(asset->name +".json");
		//cereal::JSONOutputArchive archive(os);
		std::ofstream os(asset->name + ".asset", std::ios::binary);
		cereal::BinaryOutputArchive archive(os);
		archive(asset);
	}
}

void ReadAssets(const std::string & path)
{
	std::ifstream os(path, std::ios::binary);
	cereal::BinaryInputArchive archive(os);
	shared_ptr<Assets> asset(new Assets);
	archive(asset);
	ResourceManager::AddAssets(asset);
}

void MakePrefab(shared_ptr<GameObject> target)
{
	{
		/*std::ofstream os(target->GetName()+".json");
		cereal::JSONOutputArchive archive(os);*/
		std::ofstream os(target->GetName() + ".prefab", std::ios::binary);
		cereal::BinaryOutputArchive archive(os);
		ASSERT(target->GetTransform().lock()->GetParent() == nullptr, "被复制的对象必须在根节点下");
		archive(target);
	}
}

void ReadPrefab(const std::string & path)
{
	std::ifstream os(path, std::ios::binary);
	cereal::BinaryInputArchive archive(os);
	shared_ptr<GameObject> target;
	archive(target);
}