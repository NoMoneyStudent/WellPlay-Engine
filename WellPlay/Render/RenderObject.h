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

		StructuredBuffer* m_VertexBuffer;
		ByteAddressBuffer* m_IndexBuffer;
	};

}
