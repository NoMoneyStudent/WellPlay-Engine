#include "stdafx.h"
#include "ResourceManager.h"
#include "Render\GpuBuffer.h"
#include <map>

using namespace std;

vector<shared_ptr<Assets>> assetList;
vector<shared_ptr<Mesh>> MeshList;
vector<shared_ptr<Avatar>> avatarList;
vector<shared_ptr<AnimationClip>> aniClips;
vector<pair<string, shared_ptr<StructuredBuffer>>> vertexBuffer;
vector<pair<string, shared_ptr<ByteAddressBuffer>>> indexBuffer;

bool ResourceManager::checkifExit(const std::string & name)
{
	for (int i = 0; i < MeshList.size(); i++)
	{
		if (name == MeshList[i]->name)
			return true;
	}
	
	return false;
}

void ResourceManager::AddMesh(shared_ptr<Mesh> mesh, bool hasBone)
{
	shared_ptr<StructuredBuffer> vb(new StructuredBuffer());
	shared_ptr<ByteAddressBuffer> ib(new ByteAddressBuffer());
	vertexBuffer.push_back(make_pair(mesh->name, vb));
	indexBuffer.push_back(make_pair(mesh->name, ib));

	if (hasBone)
	{
		shared_ptr<AniMesh> animesh = static_pointer_cast<AniMesh>(mesh);
		vb->Create(L"VertexBuffer" + MakeWStr(mesh->name), animesh->vertexs.size(), sizeof(AniVertex) , animesh->vertexs.data());
	}
	else
	{
		shared_ptr<CommonMesh> commonmesh = static_pointer_cast<CommonMesh>(mesh);
		vb->Create(L"VertexBuffer" + MakeWStr(mesh->name), commonmesh->vertexs.size(), sizeof(CommonVertex), commonmesh->vertexs.data());
	}
	ib->Create(L"IndexBuffer" + MakeWStr(mesh->name), mesh->indexs.size(), sizeof(UINT), mesh->indexs.data());
	MeshList.push_back(mesh);
}

void ResourceManager::GetMeshGpuBuffer(const std::string& name, shared_ptr<StructuredBuffer>& vb, shared_ptr<ByteAddressBuffer>& ib)
{
	for (int i = 0; i < vertexBuffer.size(); i++)
	{
		if (name == vertexBuffer[i].first)
		{
			vb = (vertexBuffer[i].second);
			ib = (indexBuffer[i].second);
			return;
		}
	}
	ASSERT(false, "没有找到资源?");
}

Mesh * ResourceManager::GetMesh(const std::string & name)
{
	for (auto& iter : MeshList)
	{
		if (iter->name == name)
			return iter.get();
	}
	ASSERT(false, "没找到资源？");
	return nullptr;
}

void ResourceManager::AddAvatar(shared_ptr<Avatar> avatar)
{
	avatarList.push_back(avatar);
}

Avatar * ResourceManager::GetAvatar(const std::string & name)
{
	for (auto& iter : avatarList)
	{
		if (iter->name == name)
			return iter.get();
	}
	ASSERT(false, "没找到资源？");
	return nullptr;
}

void ResourceManager::AddAnimation(shared_ptr<AnimationClip> clip)
{
	aniClips.push_back(clip);
}

AnimationClip* ResourceManager::GetAnimation(int index)
{
	return aniClips[index].get();
}

void ResourceManager::AddAssets(std::shared_ptr<Assets> asset)
{
	for (auto& iter : assetList)
	{
		if (iter->name == asset->name)
			return;
	}
	for (auto& iter : asset->meshlist)
	{
		AddMesh(iter.first, iter.second);
	}
	for (auto& iter : asset->avatarlist)
	{
		AddAvatar(iter);
	}
	for (auto& iter : asset->cliplist)
	{
		AddAnimation(iter);
	}
	assetList.push_back(asset);
}

void ResourceManager::ShutDown()
{
	assetList.clear();
	MeshList.clear();
	avatarList.clear();
	aniClips.clear();
	vertexBuffer.clear();
	indexBuffer.clear();
}
