#include "stdafx.h"
#include "ResourceManager.h"
#include "Render\GpuBuffer.h"

using namespace std;

vector<AniMesh> aniMeshList;
vector<CommonMesh> commonMeshList;
vector<Avatar> avatarList;
vector<AnimationClip> aniClips;
vector<pair<string,StructuredBuffer>> vertexBuffer;
vector<pair<string,ByteAddressBuffer>> indexBuffer;

bool ResourceManager::checkifExit(const std::string & name)
{
	for (int i = 0; i < aniMeshList.size(); i++)
	{
		if (name == aniMeshList[i].name)
			return true;
	}
	for (int i = 0; i < commonMeshList.size(); i++)
	{
		if (name == commonMeshList[i].name)
			return true;
	}
	return false;
}

void ResourceManager::AddMesh(AniMesh& mesh)
{
	StructuredBuffer m_VertexBuffer;
	ByteAddressBuffer m_IndexBuffer;
	m_VertexBuffer.Create(L"VertexBuffer"+ MakeWStr(mesh.name), mesh.vertexs.size(), sizeof(AniVertex), mesh.vertexs.data());
	m_IndexBuffer.Create(L"IndexBuffer"+ MakeWStr(mesh.name), mesh.indexs.size(), sizeof(UINT), mesh.indexs.data());
	vertexBuffer.push_back(make_pair(mesh.name, std::move(m_VertexBuffer)));
	indexBuffer.push_back(make_pair(mesh.name, std::move(m_IndexBuffer)));
	aniMeshList.push_back(std::move(mesh));
}

void ResourceManager::AddMesh(CommonMesh& mesh)
{
	StructuredBuffer m_VertexBuffer;
	ByteAddressBuffer m_IndexBuffer;
	m_VertexBuffer.Create(L"VertexBuffer" + MakeWStr(mesh.name), mesh.vertexs.size(), sizeof(CommonVertex), mesh.vertexs.data());
	m_IndexBuffer.Create(L"IndexBuffer" + MakeWStr(mesh.name), mesh.indexs.size(), sizeof(UINT), mesh.indexs.data());
	vertexBuffer.push_back(make_pair(mesh.name, std::move(m_VertexBuffer)));
	indexBuffer.push_back(make_pair(mesh.name, std::move(m_IndexBuffer)));
	commonMeshList.push_back(std::move(mesh));
}

void ResourceManager::GetMeshGpuBuffer(const std::string& name, StructuredBuffer** vb, ByteAddressBuffer** ib)
{
	for (int i = 0; i < vertexBuffer.size(); i++)
	{
		if (name == vertexBuffer[i].first)
		{
			*vb = &(vertexBuffer[i].second);
			*ib = &(indexBuffer[i].second);
			return;
		}
	}
	ASSERT(false, "没有找到资源");
}

AniMesh * ResourceManager::GetAniMesh(const std::string & name)
{
	for (AniMesh& iter : aniMeshList)
	{
		if (iter.name == name)
			return &iter;
	}
	return nullptr;
}

CommonMesh * ResourceManager::GetCommonMesh(const std::string & name)
{
	for (CommonMesh& iter : commonMeshList)
	{
		if (iter.name == name)
			return &iter;
	}
	return nullptr;
}

void ResourceManager::AddAvatar(Avatar& avatar)
{
	avatarList.push_back(std::move(avatar));
}

Avatar * ResourceManager::GetAvatar(const std::string & name)
{
	for (Avatar& iter : avatarList)
	{
		if (iter.name == name)
			return &iter;
	}
	return nullptr;
}

void ResourceManager::AddAnimation(AnimationClip& clip)
{
	aniClips.push_back(std::move(clip));
}

AnimationClip& ResourceManager::GetAnimation(int index)
{
	return aniClips[0];
}