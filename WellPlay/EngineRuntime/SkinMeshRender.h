#pragma once
#include "Render.h"
#include "Transform.h"

class SkinMeshRender :public Render
{
	friend class GameObject;
public:
	Avatar* GetAvatar() { return m_avatar; }
	void SetAvatar(Avatar* avatar);
	AniMesh* GetMesh() { return m_mesh; }
	void SetMesh(AniMesh* mesh);

private:
	Avatar* m_avatar;
	AniMesh* m_mesh;

	std::vector<Transform*> m_bones;
	std::vector<XMFLOAT4X4> m_TransformMatrix;

	SkinMeshRender();
	virtual ~SkinMeshRender();
	virtual void OnInit() override;
	virtual void Update() override;
	virtual Component* Clone()override;
};