#pragma once
#include "..\stdafx.h"
#include "DuiLib\duilib.h"

#define DuilibWnd
namespace EditorWindows
{
#ifdef DuilibWnd
	class DXWindows : public WindowImplBase
	{
	public:
		explicit DXWindows(LPCTSTR pszXMLPath);

		LPCTSTR GetWindowClassName() const;
		CDuiString GetSkinFile();
		CDuiString GetSkinFolder();

		void InitWindow();
		//void Notify(TNotifyUI& msg);
		LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	private:
		CDuiString		m_strXMLPath;
	};
#else
	class DXWindows
	{
	public:
		explicit DXWindows(std::wstring pszXMLPath, HINSTANCE);

		LPCTSTR GetWindowClassName() const { return m_strXMLPath.data(); }

		HWND GetHWND() const { return m_hwnd; }
		void ShowWindow();

	private:
		std::wstring		m_strXMLPath;
		HWND m_hwnd;
		//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
#endif // DuilibWnd
}