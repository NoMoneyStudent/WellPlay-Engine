#pragma once
#include "Resource\Model.h"
#include "Render\GpuBuffer.h"

namespace ResourceManager
{
	bool checkifExit(const std::string& name);
	void AddMesh(AniMesh& mesh);
	void AddMesh(CommonMesh& mesh);
	AniMesh* GetAniMesh(const std::string& name);
	CommonMesh* GetCommonMesh(const std::string& name);
	void AddAvatar(Avatar& avatar);
	Avatar* GetAvatar(const std::string& name);
	void AddAnimation(AnimationClip& clip);
	void GetMeshGpuBuffer(const std::string& name, StructuredBuffer** vb, ByteAddressBuffer** ib);
	AnimationClip& GetAnimation(int index);
}