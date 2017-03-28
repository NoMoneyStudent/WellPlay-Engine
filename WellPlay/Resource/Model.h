#pragma once
#include "stdafx.h"
#include <array>

typedef std::vector<std::array<float, 2>> Curve;
typedef Curve CurveXYZ[3];

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT2 UV0;
	DirectX::XMFLOAT2 UV1;
	std::vector<float> BoneWeights;
	std::vector<UINT> BoneIndex;
};

struct Animation
{
	CurveXYZ T;
	CurveXYZ R;
	CurveXYZ S;
};

struct Bone
{
	std::string name;
	int parentindex;
	float preRotation[3];

	DirectX::XMFLOAT4X4 SRT;
	DirectX::XMFLOAT4X4 Bind;

	std::vector<Animation> Animaions;
};

struct Material
{

};

struct Mesh
{
	std::vector<Vertex> vertexs;
	std::vector<UINT> indexs;

	int materialindex;
	int parentindex;
};

struct Model
{
	std::vector<Mesh> meshs;
	std::vector<Bone> bones;
	std::vector<Material> materials;
};