#include "stdafx.h"
#include "SkinMeshRender.h"
#include "Render\RenderObject.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "RenderCore.h"

using namespace RenderCore;
using namespace DirectX;
using namespace std;

void SkinMeshRender::SetAvatar(Avatar * avatar)
{
	if (m_avatar == avatar)
		return;

	m_avatar = avatar;
	FindBoneTransform();
	InitBoneMatrix();
}

void SkinMeshRender::SetMesh(Mesh * mesh)
{
	AniMesh* animesh = static_cast<AniMesh*>(mesh);
	if (m_mesh == animesh)
		return;

	m_mesh = animesh;
	ResourceManager::GetMeshGpuBuffer(m_mesh->name, render->m_VertexBuffer, render->m_IndexBuffer);
	render->indexCount = m_mesh->indexs.size();
	render->indexStride = sizeof(UINT);
	render->vertexCount = m_mesh->vertexs.size();
	render->vertexStride = sizeof(AniVertex);
	render->name = m_mesh->name;
}

void SkinMeshRender::InitBoneMatrix()
{
	m_TransformMatrix = std::vector<XMFLOAT4X4>(m_avatar->bonelists.size());

	XMStoreFloat4x4(&render->model, XMMatrixIdentity());
	render->BoneCount = m_avatar->bonelists.size();
	render->BoneTransforms = m_TransformMatrix.data();
}

void SkinMeshRender::FindBoneTransform()
{
	m_bones.clear();
	m_bones.reserve(m_avatar->bonelists.size());
	for (int i = 0; i < m_avatar->bonelists.size(); i++)
	{
		auto root = gameobject()->FindRootParent();
		ASSERT(!root.expired(), "¹Ç÷À½ÚµãÈ±Ê§ÁË");
		auto tempT = root.lock()->FindChild(m_avatar->bonelists[i].name);
		if (!tempT.expired())
		{
			m_bones.push_back(tempT.lock()->GetTransform());
		}
		else
		{
			//EditorWindows::LogWnd::Print(L"¹Ç÷À½ÚµãÈ±Ê§:  " + MakeWStr(m_avatar->bonelists[i].name));
			m_bones.push_back(weak_ptr<Transform>());
		}
	}
}

void SkinMeshRender::OnInit()
{

}

void SkinMeshRender::EditorOnInit()
{

}

void SkinMeshRender::Update()
{	
	for (int i = 0; i < m_bones.size(); i++)
	{
		XMMATRIX Bind = XMLoadFloat4x4(&m_avatar->bonelists[i].Bind);
		if (!m_bones[i].expired())
			XMStoreFloat4x4(&m_TransformMatrix[i], Bind * m_bones[i].lock()->GetWorldTranslationMatrix());
	}
	renderQueue.push_back(render);
}

void SkinMeshRender::EditorUpdate()
{
	Update();
}

Component * SkinMeshRender::Clone()
{
	SkinMeshRender* copy = new SkinMeshRender();
	copy->m_avatar = m_avatar;
	copy->m_mesh = m_mesh;
	copy->render = render;

	return static_cast<Component*>(copy);
}