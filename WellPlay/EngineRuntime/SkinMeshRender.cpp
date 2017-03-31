#include "stdafx.h"
#include "SkinMeshRender.h"
#include "Render\RenderObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "RenderCore.h"

using namespace Render;
using namespace DirectX;

void SkinMeshRender::SetMesh(AniMesh * mesh)
{
	m_mesh = mesh;
}

SkinMeshRender::SkinMeshRender()
{
}

SkinMeshRender::~SkinMeshRender()
{
}

void SkinMeshRender::Update()
{
	RenderObject render;
	render.indexCount = m_mesh->indexs.size();
	Transform* transform = gameobject()->GetTransform();
	XMStoreFloat4x4(&render.model, transform->GetWorldTranslationMatrix());
	ResourceManager::GetMeshGpuBuffer(m_mesh->name, &render.m_VertexBuffer, &render.m_IndexBuffer);
	renderQueue.push_back(std::move(render));
}

Component * SkinMeshRender::Clone()
{
	return nullptr;
}
