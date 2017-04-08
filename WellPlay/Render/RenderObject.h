#pragma once
#include "stdafx.h"
#include "Resource\TextureManager.h"
#include "GpuBuffer.h"
#include "Resource\Model.h"

namespace RenderCore
{
	using namespace DirectX;

	struct RenderObject
	{
		struct BoundingBox
		{
			XMFLOAT3 min;
			XMFLOAT3 max;
		};

		unsigned int vertexStride;
		unsigned int vertexCount;
		unsigned int indexCount;
		unsigned int indexStride;

		XMFLOAT4X4 model;
		XMFLOAT4X4* BoneTransforms;
		unsigned int BoneCount;
		std::string name;

		std::shared_ptr<StructuredBuffer> m_VertexBuffer;
		std::shared_ptr<ByteAddressBuffer> m_IndexBuffer;
	};

}
