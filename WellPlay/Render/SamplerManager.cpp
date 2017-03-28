#include "stdafx.h"
#include "SamplerManager.h"
#include "GraphicsCore.h"
#include "Utility\Hash.h"
#include <map>

using namespace std;
using Graphics::g_Device;

namespace
{
	map< size_t, D3D12_CPU_DESCRIPTOR_HANDLE > s_SamplerCache;
}

void SamplerDescriptor::Create( const D3D12_SAMPLER_DESC& Desc )
{
	size_t hashValue = Utility::HashState(&Desc);
	auto iter = s_SamplerCache.find(hashValue);
	if (iter != s_SamplerCache.end())
	{
		*this = SamplerDescriptor(iter->second);
		return;
	}

	m_hCpuDescriptorHandle = Graphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	g_Device->CreateSampler(&Desc, m_hCpuDescriptorHandle);
}
