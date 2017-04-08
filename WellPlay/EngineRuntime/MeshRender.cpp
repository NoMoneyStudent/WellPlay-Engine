#include "stdafx.h"
#include "MeshRender.h"
#include "Render\RenderObject.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "RenderCore.h"

using namespace RenderCore;

void MeshRender::SetMesh(Mesh * mesh)
{
	CommonMesh* commonmesh = static_cast<CommonMesh*>(mesh);
	if (m_mesh == commonmesh)
		return;

	m_mesh = commonmesh;
	ResourceManager::GetMeshGpuBuffer(m_mesh->name, render.m_VertexBuffer, render.m_IndexBuffer);
	render.indexCount = m_mesh->indexs.size();
	render.indexStride = sizeof(UINT);
	render.vertexCount = m_mesh->vertexs.size();
	render.vertexStride = sizeof(CommonVertex);
	render.BoneCount = 0;
	render.BoneTransforms = nullptr;
	render.name = m_mesh->name;
}

void MeshRender::OnInit()
{
}

void MeshRender::EditorOnInit()
{
}

void MeshRender::Update()
{
    XMStoreFloat4x4(&render.model, gameobject()->GetTransform().lock()->GetWorldTranslationMatrix());
	renderQueue.push_back(&render);
}

void MeshRender::EditorUpdate()
{
	Update();
}

Component * MeshRender::Clone()
{
	MeshRender* copy = new MeshRender();
	copy->m_mesh = m_mesh;
	copy->render = render;

	return static_cast<Component*>(copy);
}

MeshRender::MeshRender()
	:Render::Render()
{
}

MeshRender::~MeshRender()
{
}