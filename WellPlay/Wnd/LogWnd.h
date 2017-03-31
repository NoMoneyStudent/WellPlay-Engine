#pragma once
#include "Duilib\duilib.h"
#include "stdafx.h"

namespace EditorWindows
{
	class LogWnd : public WindowImplBase
	{
	public:
		explicit LogWnd(LPCTSTR pszXMLPath);
		~LogWnd();

		LPCTSTR GetWindowClassName() const;
		CDuiString GetSkinFile();
		CDuiString GetSkinFolder();

		void InitWindow();
		void Notify(TNotifyUI& msg);
		LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		static void Print(const std::wstring& info);

	private:
		CDuiString		m_strXMLPath;
		static CContainerUI* ground ;
	};
}