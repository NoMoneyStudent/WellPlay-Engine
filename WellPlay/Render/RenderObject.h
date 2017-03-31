#pragma once
#include "stdafx.h"
#include "Resource\TextureManager.h"
#include "GpuBuffer.h"
#include "Resource\Model.h"

namespace Render
{
	using namespace DirectX;

	struct RenderObject
	{
	public:
		RenderObject();
		~RenderObject();

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

		StructuredBuffer* m_VertexBuffer;
		ByteAddressBuffer* m_IndexBuffer;

	private:

		void ComputeBoundingBox(unsigned int meshIndex, BoundingBox &bbox) const;
	};

}
