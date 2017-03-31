#pragma once
#include "Resource\Model.h"

namespace ResourceManager
{
	bool checkifExit(const std::string& name);
	void AddMesh(AniMesh& mesh);
	void AddMesh(CommonMesh& mesh);
	AniMesh* GetAniMesh(const std::string& name);
	CommonMesh* GetCommonMesh(const std::string& name);
	void AddAvatar(Avatar& avatar);
	void AddAnimation(AnimationClip& clip);
	void GetMeshGpuBuffer(const std::string& name, StructuredBuffer** vb, ByteAddressBuffer** ib);
}