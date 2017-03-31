#pragma once
#include "Component.h"
#include "Resource\Model.h"

class SkinMeshRender :public Component
{
	friend class GameObject;
public:
	AniMesh* GetMesh() { return m_mesh; }
	void SetMesh(AniMesh* mesh);

private:
	AniMesh* m_mesh;

	SkinMeshRender();
	virtual ~SkinMeshRender();
	virtual void Update() override;
	virtual Component* Clone()override;
};