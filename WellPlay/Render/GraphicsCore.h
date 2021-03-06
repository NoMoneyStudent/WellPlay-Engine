#pragma once
#include "PipelineState.h"
#include "DescriptorHeap.h"
#include "RootSignature.h"
#include "SamplerManager.h"
#include "Resource\Model.h"

class ColorBuffer;
class GraphicsPSO;
class CommandContext;
class CommandListManager;
class CommandSignature;
class ContextManager;

namespace Graphics
{
#ifndef RELEASE
	extern const GUID WKPDID_D3DDebugObjectName;
#endif

	using namespace Microsoft::WRL;

	void Initialize(void);
	void Resize(uint32_t width, uint32_t height);
	void Terminate(void);
	void Shutdown(void);
	void Present(void);
	void Update(void);

	extern uint32_t g_DisplayWidth;
	extern uint32_t g_DisplayHeight;

	uint64_t GetFrameCount(void);
	float GetFrameTime(void);
	float GetFrameRate(void);

	extern HWND m_hwnd;
	extern std::vector<int> RenderObjects;
	extern ID3D12Device* g_Device;
	extern CommandListManager g_CommandManager;
	extern ContextManager g_ContextManager;

	extern D3D_FEATURE_LEVEL g_D3DFeatureLevel;
	extern bool g_bTypedUAVLoadSupport_R11G11B10_FLOAT;

	extern SamplerDesc SamplerLinearWrapDesc;
	extern SamplerDesc SamplerAnisoWrapDesc;
	extern SamplerDesc SamplerShadowDesc;
	extern SamplerDesc SamplerLinearClampDesc;
	extern SamplerDesc SamplerVolumeWrapDesc;
	extern SamplerDesc SamplerPointClampDesc;
	extern SamplerDesc SamplerPointBorderDesc;
	extern SamplerDesc SamplerLinearBorderDesc;

	extern SamplerDescriptor SamplerLinearWrap;
	extern SamplerDescriptor SamplerAnisoWrap;
	extern SamplerDescriptor SamplerShadow;
	extern SamplerDescriptor SamplerLinearClamp;
	extern SamplerDescriptor SamplerVolumeWrap;
	extern SamplerDescriptor SamplerPointClamp;
	extern SamplerDescriptor SamplerPointBorder;
	extern SamplerDescriptor SamplerLinearBorder;

	extern D3D12_RASTERIZER_DESC RasterizerDefault;
	extern D3D12_RASTERIZER_DESC RasterizerDefaultCW;
	extern D3D12_RASTERIZER_DESC RasterizerTwoSided;
	extern D3D12_RASTERIZER_DESC RasterizerShadow;
	extern D3D12_RASTERIZER_DESC RasterizerShadowCW;

	extern D3D12_BLEND_DESC BlendNoColorWrite;		// XXX
	extern D3D12_BLEND_DESC BlendDisable;			// 1, 0
	extern D3D12_BLEND_DESC BlendPreMultiplied;		// 1, 1-SrcA
	extern D3D12_BLEND_DESC BlendTraditional;		// SrcA, 1-SrcA
	extern D3D12_BLEND_DESC BlendAdditive;			// 1, 1
	extern D3D12_BLEND_DESC BlendTraditionalAdditive;// SrcA, 1

	extern D3D12_DEPTH_STENCIL_DESC DepthStateDisabled;
	extern D3D12_DEPTH_STENCIL_DESC DepthStateReadWrite;
	extern D3D12_DEPTH_STENCIL_DESC DepthStateReadOnly;
	extern D3D12_DEPTH_STENCIL_DESC DepthStateReadOnlyReversed;
	extern D3D12_DEPTH_STENCIL_DESC DepthStateTestEqual;

	extern CommandSignature DispatchIndirectCommandSignature;
	extern CommandSignature DrawIndirectCommandSignature;

	extern DescriptorAllocator g_DescriptorAllocator[];
	inline D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor( D3D12_DESCRIPTOR_HEAP_TYPE Type, UINT Count = 1 )
	{
		return g_DescriptorAllocator[Type].Allocate(Count);
	}

	extern RootSignature g_GenerateMipsRS;
	extern ComputePSO g_GenerateMipsLinearPSO[4];
	extern ComputePSO g_GenerateMipsGammaPSO[4];
}