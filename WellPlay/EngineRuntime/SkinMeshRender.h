#pragma once
#include "Render.h"
#include "Transform.h"
#include "ResourceManager.h"

class SkinMeshRender :public Render
{
	friend class GameObject;
	friend class cereal::access;

public:
	SkinMeshRender();
	virtual ~SkinMeshRender();
	
	Avatar* GetAvatar() { return m_avatar; }
	void SetAvatar(Avatar* avatar);
	AniMesh* GetMesh() { return m_mesh; }
	void SetMesh(Mesh* mesh);

private:
	Avatar* m_avatar;
	AniMesh* m_mesh;

	std::vector<std::weak_ptr<Transform>> m_bones;
	std::vector<XMFLOAT4X4> m_TransformMatrix;

	void InitBoneMatrix();
	void FindBoneTransform();

	virtual void OnInit() override;
	virtual void EditorOnInit() override;
	virtual void Update() override;
	virtual void EditorUpdate() override;
	virtual Component* Clone()override;

#pragma region –Ú¡–ªØ
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(m_bones, m_mesh->name,m_avatar->name);
		archive(cereal::base_class<Render>(this));
	}

	template<class Archive>
	void load(Archive & archive)
	{
		std::string meshname, avatarname;
		archive(m_bones,meshname,avatarname);
		archive(cereal::base_class<Render>(this));
		SetMesh(ResourceManager::GetMesh(meshname));
		//SetAvatar(ResourceManager::GetAvatar(avatarname));
		m_avatar=ResourceManager::GetAvatar(avatarname);
		InitBoneMatrix();
	}
#pragma endregion
};