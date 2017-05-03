#pragma once
#include <array>
#include <memory>
#include <utility>
#include <DirectXMath.h>

typedef unsigned int UINT;

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
	//float BoneWeights[4] = { 0,0,0,0 };
	std::array<float, 4> BoneWeights = { 0,0,0,0 };
	//uint32_t BoneIndex[4] = { 65536,65536,65536,65536 };
	std::array<uint32_t, 4> BoneIndex = { 65536,65536,65536,65536 };
};

struct Animation
{
	std::vector<std::pair<DirectX::XMFLOAT3, double>> T;
	std::vector<std::pair<DirectX::XMFLOAT4, double>> R;
	std::vector<std::pair<DirectX::XMFLOAT3, double>> S;
};

struct Bone
{
	std::string name;
	DirectX::XMFLOAT4X4 Bind;
};

struct Avatar
{
	std::string name;
	std::vector<Bone> bonelists;
};

struct AnimationClip
{
	std::vector<std::pair<std::string, Animation>> clips;
	float durning;
	bool loop = false;
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

struct Mesh
{
	std::string name;
	std::vector<UINT> indexs;
	std::vector<Material*> materials;
	
	virtual ~Mesh() {};
};

struct CommonMesh:public Mesh
{
	std::vector<CommonVertex> vertexs;

	virtual ~CommonMesh() {};
};

struct AniMesh:public Mesh
{
	std::vector<AniVertex> vertexs;

	virtual ~AniMesh() {};
};

struct Assets
{
	std::string name;
	std::vector<std::pair<std::shared_ptr<Mesh>,bool>> meshlist;
	std::vector<std::shared_ptr<Avatar>> avatarlist;
	std::vector<std::shared_ptr<AnimationClip>> cliplist;
};