#pragma once

#include "stdafx.h"
#include "DXWindows.h"
#include "EditorMainWnd.h"

class Editor
{
public:
	Editor(HINSTANCE hInstance);
	~Editor();

	void Update();

	HWND GetDXWndHwnd() const { return DXWnd->GetHWND(); }

private:

	void CenterWindows();

	EditorWindows::EditorMainWnd* EditorWnd;
	EditorWindows::DXWindows* DXWnd;
};

inline Editor::Editor(HINSTANCE hInstance)
{
	::CoInitialize(NULL);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	EditorWnd = new EditorWindows::EditorMainWnd(_T("EditorSkin.xml"));
	EditorWnd->Create(NULL, _T("EditorMain"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	EditorWnd->ShowWindow();

	DXWnd = new EditorWindows::DXWindows(_T("DXSkin.xml"));
	DXWnd->Create(NULL, _T("RenderWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	DXWnd->ShowWindow();

	CenterWindows();
}

inline Editor::~Editor()
{
	delete DXWnd;
	delete EditorWnd;

	::CoUninitialize();
}
