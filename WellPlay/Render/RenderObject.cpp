#include "stdafx.h"
#include "RenderObject.h"
#include <float.h>

namespace Render
{
	RenderObject::RenderObject()
	{
	}

	RenderObject::~RenderObject()
	{
	}

	/*void RenderObject::Create(Mesh& mesh, std::vector<Bone>& bone,int materialindex)
	{
		boneList = &bone;

		vertexs.reserve(mesh.vertexs.size());
		float minx=100;
		float miny=100;
		float maxx=-100;
		float maxy=-100;
		for (int i = 0; i < mesh.vertexs.size(); i++)
		{
			ASSERT(mesh.vertexs[i].BoneIndex.size() <= 4, "BoneNum is out of range!!");
			ASSERT(mesh.vertexs[i].BoneIndex.size() == mesh.vertexs[i].BoneWeights.size(), "Model Data is bad !!");
			RenderVertex rv;
			rv.pos = mesh.vertexs[i].pos;
			rv.color = mesh.vertexs[i].color;
			rv.normal = mesh.vertexs[i].normal;
			rv.tangent = mesh.vertexs[i].tangent;
			rv.UV0 = mesh.vertexs[i].UV0;
			rv.UV1 = mesh.vertexs[i].UV1;
			if (rv.UV0.x > 1)
				rv.UV0.x -= 1;
			int j;
			for (j = 0; j < mesh.vertexs[i].BoneIndex.size(); j++)
			{
				rv.BoneWeights[j] = mesh.vertexs[i].BoneWeights[j];
				rv.BoneIndex[j] = mesh.vertexs[i].BoneIndex[j];
			}
			for (; j < 4; j++)
			{
				rv.BoneIndex[j] = 65536;
			}
			vertexs.push_back(rv);
		}
		indices.reserve(mesh.indexs.size());
		for (int i = 0; i < mesh.indexs.size(); i++)
		{
			indices.push_back(mesh.indexs[i]);
		}

		materialIndex = materialindex;
		
		vertexStride = sizeof(RenderVertex);
		vertexCount = vertexs.size();
		indexStride = sizeof(UINT);
		indexCount = indices.size();

		m_VertexBuffer.Create(L"VertexBuffer", vertexCount, vertexStride, vertexs.data());
		m_IndexBuffer.Create(L"IndexBuffer", indexCount, indexStride, indices.data());
	}
*/

}
