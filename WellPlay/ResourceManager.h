#pragma once
#include "Resource\Model.h"
#include "Render\GpuBuffer.h"
#include <memory>

namespace ResourceManager
{
	bool checkifExit(const std::string & name);
	void AddMesh(std::shared_ptr<Mesh> mesh, bool hasBone = true);
	Mesh * GetMesh(const std::string & name);
	void AddAvatar(std::shared_ptr<Avatar> avatar);
	Avatar * GetAvatar(const std::string & name);
	void AddAnimation(std::shared_ptr<AnimationClip> clip);
	void GetMeshGpuBuffer(const std::string& name, std::shared_ptr<StructuredBuffer>& vb, std::shared_ptr<ByteAddressBuffer>& ib);
	AnimationClip* GetAnimation(int index);
	void AddAssets(std::shared_ptr<Assets> asset);

	void ShutDown();
}