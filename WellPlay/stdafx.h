// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//#include <cstring>
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Utility\Utility.h"

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL      ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "Render\d3dx12.h"
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <pix.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define MY_IID_PPV_ARGS IID_PPV_ARGS

//
// The DXGI tearing feature is available on Windows 10 systems with KB3156421 or the
// Anniversary Update installed. Since the 10586 SDK will not be patched, we define
// the relevant symbols if they are missing.
//
#ifdef DXGI_PRESENT_ALLOW_TEARING
#include <dxgi1_5.h>
#else
#define DXGI_PRESENT_ALLOW_TEARING          0x00000200UL
#define DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING  2048

typedef
enum DXGI_FEATURE
{
	DXGI_FEATURE_PRESENT_ALLOW_TEARING = 0
} DXGI_FEATURE;

MIDL_INTERFACE("7632e1f5-ee65-4dca-87fd-84cd75f8838d")
IDXGIFactory5 : public IDXGIFactory4
{
public:
	virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
		DXGI_FEATURE Feature,
		_Inout_updates_bytes_(FeatureSupportDataSize) void *pFeatureSupportData,
		UINT FeatureSupportDataSize) = 0;
};
#endif

/*����һ����Ԫ��תŷ���ǵĺ���*/
namespace DirectX
{
	inline XMFLOAT3 XMQuaternion2Euler(FXMVECTOR q)
	{
		return XMFLOAT3
		{
			atan2(2 * (q.m128_f32[3] * q.m128_f32[2] + q.m128_f32[0] * q.m128_f32[1]),1 - 2 * (q.m128_f32[2] * q.m128_f32[2] + q.m128_f32[0] * q.m128_f32[0]))*57.3f,
			asin(2 * (q.m128_f32[3] * q.m128_f32[0] - q.m128_f32[1] * q.m128_f32[2]))*57.3f,
			atan2(2 * (q.m128_f32[3] * q.m128_f32[1] + q.m128_f32[2] * q.m128_f32[0]),1 - 2 * (q.m128_f32[1] * q.m128_f32[1] + q.m128_f32[0] * q.m128_f32[0]))*57.3f
		};
	}

	inline XMVECTOR XMQuaternionRotate(FXMVECTOR source, FXMVECTOR rotation)
	{
		XMVECTOR result;
		result = XMQuaternionMultiply(source, rotation);
		result = XMQuaternionMultiply(XMQuaternionInverse(rotation), source);
		return result;
	}
}