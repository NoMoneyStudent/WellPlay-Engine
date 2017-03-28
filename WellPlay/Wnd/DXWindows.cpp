#include "stdafx.h"
#include "DXWindows.h"
#include "RenderCore.h"

using namespace EditorWindows;

#ifdef DuilibWnd
DXWindows::DXWindows(LPCTSTR pszXMLPath)
	: m_strXMLPath(pszXMLPath)
{
}

LPCTSTR DXWindows::GetWindowClassName() const
{
	return _T("RenderWnd");
}

CDuiString DXWindows::GetSkinFile()
{
	return m_strXMLPath;
}

CDuiString DXWindows::GetSkinFolder()
{
	return _T("Skin");
}

void DXWindows::InitWindow()
{
}

LRESULT DXWindows::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_SIZE) 
	{
		Render::ReSize((UINT)(UINT64)lParam & 0xFFFF, (UINT)(UINT64)lParam >> 16);
	}

	bHandled = false;
	return 0;
}
#else
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool ifmove = false;
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_NCHITTEST:
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		::ScreenToClient(hWnd, &pt);

		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);

		if (pt.x<rcClient.left + 20 && pt.y<rcClient.top + 20)//左上角,判断是不是在左上角，就是看当前坐标是不是即在左边拖动的范围内，又在上边拖动的范围内，其它角判断方法类似  
		{
			return HTTOPLEFT;
		}
		else if (pt.x>rcClient.right - 20 && pt.y<rcClient.top + 20)//右上角  
		{
			return HTTOPRIGHT;
		}
		else if (pt.x<rcClient.left + 20 && pt.y>rcClient.bottom - 20)//左下角  
		{
			return HTBOTTOMLEFT;
		}
		else if (pt.x>rcClient.right - 20 && pt.y>rcClient.bottom - 20)//右下角  
		{
			return HTBOTTOMRIGHT;
		}
		else if (pt.x<rcClient.left + 20)
		{
			return HTLEFT;
		}
		else if (pt.x>rcClient.right - 20)
		{
			return HTRIGHT;
		}
		else if (pt.y<rcClient.top + 20)
		{
			return HTTOP;
		}if (pt.y>rcClient.bottom - 20)
		{
			return HTBOTTOM;          //以上这四个是上、下、左、右四个边  
		}
		else
		{
			return HTCAPTION;
		}
		break;

	case WM_SIZE:
		//Graphics::Resize((UINT)(UINT64)lParam & 0xFFFF, (UINT)(UINT64)lParam >> 16);
		break;

	case WM_DESTROY:
		//PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
	{
		if (ifmove)
			SendMessage(hWnd, WM_SYSCOMMAND, 0xF012, 0);
	}
	break;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
			ifmove = true;
		break;
	case WM_KEYUP:
		if (wParam == VK_SPACE)
			ifmove = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

DXWindows::DXWindows(std::wstring pszXMLPath, HINSTANCE hInst)
	: m_strXMLPath(pszXMLPath)
{

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = pszXMLPath.data();
	wcex.hIconSm = LoadIcon(hInst, IDI_APPLICATION);
	ASSERT(0 != RegisterClassEx(&wcex), "Unable to register a window");

	RECT rc = { 0, 0, 800, 800 };
	AdjustWindowRect(&rc, WS_POPUP, FALSE);

	m_hwnd = CreateWindow(pszXMLPath.data(), pszXMLPath.data(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInst, nullptr);

	ASSERT(m_hwnd != 0);
	//::ShowCursor(true);
}

void DXWindows::ShowWindow()
{
	::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}
#endif // DuilibWnd