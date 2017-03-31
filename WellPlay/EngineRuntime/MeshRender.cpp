#include "stdafx.h"
#include "MeshRender.h"

void MeshRender::SetMesh(CommonMesh * mesh)
{
	m_mesh = mesh;
}

MeshRender::MeshRender()
{
}

MeshRender::~MeshRender()
{
}

Component * MeshRender::Clone()
{
	return nullptr;
}
