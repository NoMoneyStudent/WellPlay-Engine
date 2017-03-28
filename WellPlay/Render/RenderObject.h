#pragma once
#include "stdafx.h"
#include "Resource\TextureManager.h"
#include "GpuBuffer.h"
#include "Resource\Model.h"

namespace Render
{
	using namespace DirectX;

	struct Material
	{
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		XMFLOAT3 ambient;
		XMFLOAT3 emissive;
		XMFLOAT3 transparent; // light passing through a transparent surface is multiplied by this filter color
		float opacity;
		float shininess; // specular exponent
		float specularStrength; // multiplier on top of specular color

		enum { maxTexPath = 128 };
		enum { texCount = 6 };
		char texDiffusePath[maxTexPath];
		char texSpecularPath[maxTexPath];
		char texEmissivePath[maxTexPath];
		char texNormalPath[maxTexPath];
		char texLightmapPath[maxTexPath];
		char texReflectionPath[maxTexPath];

		enum { maxMaterialName = 128 };
		char name[maxMaterialName];
		void ReleaseTextures();
		void LoadTextures();
	};

	struct RenderObject
	{
	public:
		RenderObject();
		~RenderObject();

		void Clear();
		void Create(Mesh& mesh, std::vector<Bone>& bone, int materialindex);

		struct BoundingBox
		{
			XMFLOAT3 min;
			XMFLOAT3 max;
		};

		BoundingBox boundingBox;
		unsigned int vertexStride;
		unsigned int vertexCount;
		unsigned int indexCount;
		unsigned int indexStride;

		unsigned int materialIndex;
		
		struct RenderVertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 UV0;
			DirectX::XMFLOAT2 UV1;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT3 tangent;
			float BoneWeights[4];
			uint32_t BoneIndex[4];
		};

		std::vector<RenderVertex> vertexs;
		std::vector<UINT> indices;
		std::vector<Bone>* boneList;

		XMFLOAT4X4 BoneTransforms[64];

		StructuredBuffer m_VertexBuffer;
		ByteAddressBuffer m_IndexBuffer;

		D3D12_CPU_DESCRIPTOR_HANDLE* GetSRVs(uint32_t materialIdx) const
		{
			return m_SRVs + materialIdx * 6;
		}

	private:

		void ComputeBoundingBox(unsigned int meshIndex, BoundingBox &bbox) const;

		D3D12_CPU_DESCRIPTOR_HANDLE* m_SRVs;
	};

}
