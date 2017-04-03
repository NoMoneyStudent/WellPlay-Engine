#pragma once
#include "Render.h"
#include "Resource\Model.h"

class MeshRender:public Render
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