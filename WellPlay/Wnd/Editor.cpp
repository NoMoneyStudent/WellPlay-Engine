#include "stdafx.h"
#include "Editor.h"

using namespace EditorWindows;

void Editor::CenterWindows()
{
	RECT EditorRect = { 0 };
	GetWindowRect(EditorWnd->GetHWND(), &EditorRect);
	RECT DXRect = { 0 };
	GetWindowRect(DXWnd->GetHWND(), &DXRect);

	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	GetMonitorInfo(::MonitorFromWindow(EditorWnd->GetHWND(), MONITOR_DEFAULTTONEAREST), &oMonitor);

	SetWindowPos(EditorWnd->GetHWND(), NULL, 0, oMonitor.rcWork.bottom / 2- EditorRect.bottom / 2, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowPos(DXWnd->GetHWND(), NULL, EditorRect.right, oMonitor.rcWork.bottom / 2 - DXRect.bottom / 2, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowPos(Log->GetHWND(), NULL, EditorRect.right+ DXRect.right, oMonitor.rcWork.bottom / 2 - DXRect.bottom / 2, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Editor::Update()
{
	CPaintManagerUI::MessageLoop();
}