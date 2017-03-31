#pragma once
#include "stdafx.h"
#include "Render\GpuBuffer.h"
#include <array>

typedef std::vector<std::array<float, 2>> Curve;
typedef Curve CurveXYZ[3];

struct CommonVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 UV0;
	DirectX::XMFLOAT2 UV1;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
};

struct AniVertex:public CommonVertex
{
	float BoneWeights[4] = { 0,0,0,0 };
	uint32_t BoneIndex[4] = { 65536,65536,65536,65536 };
};

struct Animation
{
	std::vector<std::pair<DirectX::XMFLOAT3, float>> T;
	std::vector<std::pair<DirectX::XMFLOAT4, float>> R;
	std::vector<std::pair<DirectX::XMFLOAT3, float>> S;
};

struct Bone
{
	std::string name;

	DirectX::XMFLOAT4X4 Bind;
};

struct Avatar
{
	std::vector<Bone> bonelists;
};

struct AnimationClip
{
	std::vector<std::pair<std::string, Animation>> clips;
	float durning;
	std::string name;
};

struct Material
{
	//XMFLOAT3 diffuse;
	//XMFLOAT3 specular;
	//XMFLOAT3 ambient;
	//XMFLOAT3 emissive;
	//XMFLOAT3 transparent; // light passing through a transparent surface is multiplied by this filter color
	//float opacity;
	//float shininess; // specular exponent
	//float specularStrength; // multiplier on top of specular color

	//enum { maxTexPath = 128 };
	//enum { texCount = 6 };
	//char texDiffusePath[maxTexPath];
	//char texSpecularPath[maxTexPath];
	//char texEmissivePath[maxTexPath];
	//char texNormalPath[maxTexPath];
	//char texLightmapPath[maxTexPath];
	//char texReflectionPath[maxTexPath];

	//enum { maxMaterialName = 128 };
	//char name[maxMaterialName];
	//void ReleaseTextures();
	//void LoadTextures();
};

struct AniMesh
{
	std::string name;
	std::vector<AniVertex> vertexs;
	std::vector<UINT> indexs;
	//std::vector<std::vector<UINT>> indexs;

	std::vector<Material*> materials;
	Avatar* avatra;
};

struct CommonMesh
{
	std::string name;
	std::vector<CommonVertex> vertexs;
	std::vector<UINT> indexs;
	//std::vector<std::vector<UINT>> indexs;
	
	std::vector<Material*> materials;
};