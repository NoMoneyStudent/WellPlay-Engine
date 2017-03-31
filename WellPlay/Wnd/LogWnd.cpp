#include "stdafx.h"
#include "LogWnd.h"
#include "Utility\FileUtility.h"

using namespace EditorWindows;

CContainerUI* LogWnd::ground = nullptr;

LogWnd::LogWnd(LPCTSTR pszXMLPath)
	: m_strXMLPath(pszXMLPath)
{
}

LogWnd::~LogWnd()
{
}

LPCTSTR LogWnd::GetWindowClassName() const
{
	return _T("LogWnd");
}

CDuiString LogWnd::GetSkinFile()
{
	return m_strXMLPath;
}

CDuiString LogWnd::GetSkinFolder()
{
	return _T("Skin");
}

void LogWnd::InitWindow()
{
	ground= (CContainerUI*)m_pm.FindControl(_T("print"));
}

void LogWnd::Notify(TNotifyUI& msg)
{
	__super::Notify(msg);
}

LRESULT LogWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	bHandled = false;
	return 0;
}

void LogWnd::Print(const std::wstring & info)
{
	if (ground != nullptr)
	{
		CLabelUI* l = new CLabelUI();
		l->SetText(info.data());
		ground->Add(l);
		CLabelUI* line = new CLabelUI();
		line->SetBkImage(L"hor_line.png");
		line->SetFixedHeight(1);
		ground->Add(line);
	}
}