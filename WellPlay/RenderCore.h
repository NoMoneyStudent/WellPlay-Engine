#pragma once

#include "stdafx.h"
#include "Render\RenderObject.h"

namespace RenderCore
{
	void Initialize(HWND m_hwnd);
	void Shutdown(void);

	void Update(void);
	void Render(void);

	void ReSize(UINT width,UINT height);
	UINT GetDisplayWidth();
	UINT GetDisplayHeight();
	void SetFullScreen(bool isFullScreen);

	extern std::vector<RenderObject*> renderQueue;
	extern bool isTexture;
	extern D3D12_CPU_DESCRIPTOR_HANDLE texture;
};
