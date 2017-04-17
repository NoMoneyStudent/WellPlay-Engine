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
#include "ResourceManager.h"
#include "Utility\FileUtility.h"
#include "AnimationCore.h"

using namespace Graphics;
using namespace DirectX;
using namespace std;

namespace RenderCore
{
	void RenderObjects(GraphicsContext& Context);

	D3D12_VIEWPORT m_MainViewport;
	D3D12_RECT m_MainScissor;

	RootSignature m_RootSig;
	GraphicsPSO m_DepthPSO;
	GraphicsPSO m_ModelPSO;
	GraphicsPSO m_ShadowPSO;
	D3D12_CPU_DESCRIPTOR_HANDLE m_ExtraTextures[2];
	vector<RenderObject*> renderQueue;
	bool isTexture = false;
	D3D12_CPU_DESCRIPTOR_HANDLE texture;
}

void RenderCore::Initialize(HWND m_hwnd)
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
	m_ModelPSO.SetRasterizerState(RasterizerDefaultCW);
	m_ModelPSO.SetInputLayout(_countof(vertElem), vertElem);
	m_ModelPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_ModelPSO.SetBlendState(BlendDisable);
	m_ModelPSO.SetDepthStencilState(DepthStateReadWrite);
	m_ModelPSO.SetRenderTargetFormats(1, &ColorFormat, DepthFormat);

	ByteArray m_vertexShader = FileUtility::ReadFileByte("SampleVertexShader.cso");
	ByteArray m_pixelShader = FileUtility::ReadFileByte("SamplePixelShader.cso");

	m_ModelPSO.SetVertexShader(m_vertexShader.data(), m_vertexShader.size());
	m_ModelPSO.SetPixelShader(m_pixelShader.data(), m_pixelShader.size());
	m_ModelPSO.Finalize();

	m_ExtraTextures[0] = g_SSAOFullScreen.GetSRV();
	m_ExtraTextures[1] = g_ShadowBuffer.GetSRV();

	TextureManager::Initialize(L"Textures/");
}

void RenderCore::Shutdown( void )
{
	Graphics::Terminate();
	Graphics::Shutdown();
}

void RenderCore::Update( void )
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

void RenderCore::RenderObjects(GraphicsContext& gfxContext)
{

	struct VSConstants
	{
		XMFLOAT4X4 model;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4X4 transformbones[96];
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

	static XMVECTORF32 eye = { 0.0f, 0.5f, 1.0f, 0.0f };
	static XMVECTORF32 at = { 0.0f, 0.5f, 0.0f, 0.0f };
	static XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	static float anger = 0;
	if (GameInput::IsPressed(GameInput::DigitalInput::kKey_w))
		eye.f[1] -= 0.02f;
	if (GameInput::IsPressed(GameInput::DigitalInput::kKey_s))
		eye.f[1] += 0.02f;
	if (GameInput::IsPressed(GameInput::DigitalInput::kKey_a))
		anger += XM_PI / 180;
	if (GameInput::IsPressed(GameInput::DigitalInput::kKey_d))
		anger -= XM_PI / 180;

	XMStoreFloat4x4(&vsConstants.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	while (!renderQueue.empty())
	{
		XMStoreFloat4x4(&vsConstants.model, XMMatrixTranspose(XMLoadFloat4x4(&renderQueue.back()->model)*XMMatrixRotationY(anger)));

		for (int i = 0; i < renderQueue.back()->BoneCount; i++)
		{
			XMStoreFloat4x4(&vsConstants.transformbones[i], XMMatrixTranspose(XMLoadFloat4x4(&renderQueue.back()->BoneTransforms[i])));
		}

		gfxContext.SetDynamicConstantBufferView(0, sizeof(vsConstants), &vsConstants);
		if (isTexture)
			gfxContext.SetDynamicDescriptors(3, 0, 1, &texture);
	
		gfxContext.SetIndexBuffer(renderQueue.back()->m_IndexBuffer->IndexBufferView());
		gfxContext.SetVertexBuffer(0, renderQueue.back()->m_VertexBuffer->VertexBufferView());
		gfxContext.DrawIndexed(renderQueue.back()->indexCount);
		renderQueue.pop_back();
	}
}

void RenderCore::Render(void)
{
	GraphicsContext& gfxContext = GraphicsContext::Begin(L"Scene Render");

	gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
	gfxContext.ClearColor(g_SceneColorBuffer);

	gfxContext.SetRootSignature(m_RootSig);
	gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfxContext.SetDynamicDescriptors(4, 0, 2, m_ExtraTextures);
	gfxContext.SetPipelineState(m_ModelPSO);
	//gfxContext.TransitionResource(g_SSAOFullScreen, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ);
	gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	gfxContext.ClearDepth(g_SceneDepthBuffer);
	gfxContext.SetDepthStencilTarget(g_SceneDepthBuffer.GetDSV());
	gfxContext.SetRenderTarget(g_SceneColorBuffer.GetRTV(), g_SceneDepthBuffer.GetDSV());
	gfxContext.SetViewportAndScissor(m_MainViewport, m_MainScissor);
	RenderObjects(gfxContext);

	gfxContext.Finish();
	Graphics::Present();
}

void RenderCore::ReSize(UINT width, UINT height)
{
	Graphics::Resize(width, height);
}

UINT RenderCore::GetDisplayHeight()
{
	return Graphics::g_DisplayHeight;
}

UINT RenderCore::GetDisplayWidth()
{
	return Graphics::g_DisplayWidth;
}