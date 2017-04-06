#pragma once
#include "Render.h"
#include "Transform.h"

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
	void SetMesh(AniMesh* mesh);

private:
	Avatar* m_avatar;
	AniMesh* m_mesh;

	std::vector<std::weak_ptr<Transform>> m_bones;
	std::vector<XMFLOAT4X4> m_TransformMatrix;

	virtual void OnInit() override;
	virtual void EditorOnInit() override;
	virtual void Update() override;
	virtual void EditorUpdate() override;
	virtual Component* Clone()override;
#pragma region –Ú¡–ªØ
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(m_bones);
		archive(cereal::base_class<Render>(this));
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(m_bones);
		archive(cereal::base_class<Render>(this));
	}
#pragma endregion
};