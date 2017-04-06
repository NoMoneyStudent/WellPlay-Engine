#include "stdafx.h"
#include "Serialize.h"
#include <cereal\archives\json.hpp>
#include <cereal\archives\binary.hpp>
#include <cereal\types\polymorphic.hpp>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>
#include <cereal\types\string.hpp>
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

CEREAL_REGISTER_TYPE(Transform);
CEREAL_REGISTER_TYPE(SkinMeshRender);
CEREAL_REGISTER_TYPE(MeshRender);
CEREAL_REGISTER_TYPE(Render);
CEREAL_REGISTER_TYPE(Animator);
CEREAL_REGISTER_TYPE(EditorComponent);

CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Transform)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, EditorComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Animator)
CEREAL_REGISTER_POLYMORPHIC_RELATION(EditorComponent, Render)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Render, SkinMeshRender)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Render, MeshRender)

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
}
#pragma endregion

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
	int i = 0;
}