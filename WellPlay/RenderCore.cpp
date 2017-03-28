#include "stdafx.h"
#include "Render\GraphicsCore.h"
#include "Render\BufferManager.h"
#include "Render\GpuBuffer.h"
#include "Render\CommandContext.h"
#include "Render\SamplerManager.h"
#include "Render\EngineTuning.h"
#include "Render\EngineProfiling.h"
#include "SystemTime.h"
#include "GameInput.h"
#include "RenderCore.h"
#include "Resource\Model.h"
#include "Utility\FileUtility.h"
#include "AnimationCore.h"

using namespace Graphics;
using namespace DirectX;
using namespace std;

namespace Render
{
	void RenderObjects(GraphicsContext& Context);

	D3D12_VIEWPORT m_MainViewport;
	D3D12_RECT m_MainScissor;

	RootSignature m_RootSig;
	GraphicsPSO m_DepthPSO;
	GraphicsPSO m_ModelPSO;
	GraphicsPSO m_ShadowPSO;
	D3D12_CPU_DESCRIPTOR_HANDLE m_ExtraTextures[2];
	vector<RenderObject> renderQueue;
	bool isTexture = false;
	D3D12_CPU_DESCRIPTOR_HANDLE texture;
}

void Render::Initialize(HWND m_hwnd)
{
	Graphics::m_hwnd = m_hwnd;
	Graphics::Initialize();

	m_RootSig.Reset(6, 2);
	m_RootSig.InitStaticSampler(0, SamplerAnisoWrapDesc, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig.InitStaticSampler(1, SamplerShadowDesc, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig[0].InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	m_RootSig[1].InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig[2].InitAsBufferSRV(0, D3D12_SHADER_VISIBILITY_VERTEX);
	m_RootSig[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 6, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig[4].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 64, 3, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig[5].InitAsConstants(1, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	m_RootSig.Finalize(L"StanderdAnimation", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	DXGI_FORMAT ColorFormat = g_SceneColorBuffer.GetFormat();
	DXGI_FORMAT DepthFormat = g_SceneDepthBuffer.GetFormat();
	DXGI_FORMAT ShadowFormat = g_ShadowBuffer.GetFormat();

	D3D12_INPUT_ELEMENT_DESC vertElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	m_ModelPSO.SetRootSignature(m_RootSig);
	m_ModelPSO.SetRasterizerState(RasterizerDefault);
	m_ModelPSO.SetInputLayout(_countof(vertElem), vertElem);
	m_ModelPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_ModelPSO.SetBlendState(BlendDisable);
	m_ModelPSO.SetDepthStencilState(DepthStateReadWrite);
	m_ModelPSO.SetRenderTargetFormats(1, &ColorFormat, DepthFormat);

	ByteArray m_vertexShader = FileUtility::ReadFileHelper("SampleVertexShader.cso");
	ByteArray m_pixelShader = FileUtility::ReadFileHelper("SamplePixelShader.cso");

	m_ModelPSO.SetVertexShader(m_vertexShader->data(), m_vertexShader->size());
	m_ModelPSO.SetPixelShader(m_pixelShader->data(), m_pixelShader->size());
	m_ModelPSO.Finalize();

	m_ExtraTextures[0] = g_SSAOFullScreen.GetSRV();
	m_ExtraTextures[1] = g_ShadowBuffer.GetSRV();

	TextureManager::Initialize(L"Textures/");
}

void Render::Shutdown( void )
{
	Graphics::Terminate();
	Graphics::Shutdown();
}

void Render::Update( void )
{
	ScopedTimer _prof(L"Update State");

	m_MainViewport.Width = (float)g_SceneColorBuffer.GetWidth();
	m_MainViewport.Height = (float)g_SceneColorBuffer.GetHeight();
	m_MainViewport.MinDepth = 0.0f;
	m_MainViewport.MaxDepth = 1.0f;

	m_MainScissor.left = 0;
	m_MainScissor.top = 0;
	m_MainScissor.right = (LONG)g_SceneColorBuffer.GetWidth();
	m_MainScissor.bottom = (LONG)g_SceneColorBuffer.GetHeight();
}

void Render::RenderObjects(GraphicsContext& gfxContext)
{
	struct VSConstants
	{
		XMFLOAT4X4 model;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4X4 transformbones[64];
	} vsConstants;

	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(
		60.0f * XM_PI / 180.0f,
		1,
		0.01f,
		100.0f
	);
	XMStoreFloat4x4(
		&vsConstants.projection,
		XMMatrixTranspose(perspectiveMatrix)
	);
	
	static const XMVECTORF32 eye = { 0.0f, 0.25f, -1.00f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, 0.25f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&vsConstants.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&vsConstants.model, XMMatrixTranspose(XMMatrixRotationY(0)));

	Ani(renderQueue[0]);
	vector<Bone>& BoneList = *(renderQueue[0].boneList);
	for (int i = 0; i < BoneList.size(); i++)
	{
		XMStoreFloat4x4(&vsConstants.transformbones[i], XMLoadFloat4x4(&BoneList[i].Bind) * XMLoadFloat4x4(&BoneList[i].SRT));
	}

	gfxContext.SetDynamicConstantBufferView(0, sizeof(vsConstants), &vsConstants);
	if(isTexture)
	   gfxContext.SetDynamicDescriptors(3, 0, 1, &texture);

	gfxContext.DrawIndexed(renderQueue[0].indices.size());
}

void Render::Render( void )
{
	if (renderQueue.empty())
	{
		return;
	}
	GraphicsContext& gfxContext = GraphicsContext::Begin(L"Scene Render");
	{
		ScopedTimer _prof(L"Main Render", gfxContext);

		gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
		gfxContext.ClearColor(g_SceneColorBuffer);

		// Set the default state for command lists
		auto& pfnSetupGraphicsState = [&](void)
		{
			gfxContext.SetRootSignature(m_RootSig);
			gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			gfxContext.SetIndexBuffer(renderQueue[0].m_IndexBuffer.IndexBufferView());
			gfxContext.SetVertexBuffer(0, renderQueue[0].m_VertexBuffer.VertexBufferView());
			gfxContext.SetDynamicDescriptors(4, 0, 2, m_ExtraTextures);
			//gfxContext.SetDynamicConstantBufferView(1, sizeof(XMFLOAT4X4)*64, renderQueue[0].BoneTransforms);
		};

		pfnSetupGraphicsState();

		{
			ScopedTimer _prof(L"Render Color", gfxContext);
			gfxContext.SetPipelineState(m_ModelPSO);
			//gfxContext.TransitionResource(g_SSAOFullScreen, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
			//gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ);
			gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			gfxContext.ClearDepth(g_SceneDepthBuffer);
			gfxContext.SetDepthStencilTarget(g_SceneDepthBuffer.GetDSV());
			gfxContext.SetRenderTarget(g_SceneColorBuffer.GetRTV(), g_SceneDepthBuffer.GetDSV());
			gfxContext.SetViewportAndScissor(m_MainViewport, m_MainScissor);
			RenderObjects( gfxContext);
		}
	}
	gfxContext.Finish();
	Graphics::Present();
}

void Render::ReSize(UINT width, UINT height)
{
	Graphics::Resize(width, height);
}

UINT Render::GetDisplayHeight()
{
	return Graphics::g_DisplayHeight;
}

UINT Render::GetDisplayWidth()
{
	return Graphics::g_DisplayWidth;
}