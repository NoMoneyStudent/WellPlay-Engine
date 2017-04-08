#pragma once
#include "Render.h"
#include "Resource\Model.h"
#include "ResourceManager.h"

class MeshRender:public Render
{
	friend class GameObject;
	friend class cereal::access;

public:
	MeshRender();
	virtual ~MeshRender();
	
	CommonMesh* GetMesh() { return m_mesh; }
	void SetMesh(Mesh* mesh);

private:
	CommonMesh* m_mesh;

	virtual void OnInit() override;
	virtual void EditorOnInit() override;
	virtual void Update() override;
	virtual void EditorUpdate() override;
	virtual Component* Clone()override;
#pragma region –Ú¡–ªØ
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(m_mesh->name);
		archive(cereal::base_class<Render>(this));
	}

	template<class Archive>
	void load(Archive & archive)
	{
		std::string meshname;
		archive(meshname);
		archive(cereal::base_class<Render>(this));
		SetMesh(ResourceManager::GetMesh(meshname));
	}
#pragma endregion
};