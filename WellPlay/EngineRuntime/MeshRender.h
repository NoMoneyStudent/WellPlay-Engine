#pragma once
#include "Component.h"
#include "Resource\Model.h"

class MeshRender:public Component
{
	friend class GameObject;
public:
	CommonMesh* GetMesh() { return m_mesh; }
	void SetMesh(CommonMesh* mesh);

private:
	CommonMesh* m_mesh;

	MeshRender();
	virtual ~MeshRender();

	virtual Component* Clone()override;
};