#pragma once

#include "stdafx.h"
#include "DXWindows.h"
#include "EditorMainWnd.h"
#include "LogWnd.h"

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
	EditorWindows::LogWnd* Log;
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

	Log = new EditorWindows::LogWnd(_T("LogSkin.xml"));
	Log->Create(NULL, _T("LogWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	Log->ShowWindow();

	CenterWindows();
}

inline Editor::~Editor()
{
	delete DXWnd;
	delete EditorWnd;

	::CoUninitialize();
}
