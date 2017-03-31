#pragma once
#include "Duilib\duilib.h"
#include "Resource\Model.h"
#include "stdafx.h"

namespace EditorWindows
{
	class EditorMainWnd : public WindowImplBase
	{
	public:
		explicit EditorMainWnd(LPCTSTR pszXMLPath);
		~EditorMainWnd();

		LPCTSTR GetWindowClassName() const;
		CDuiString GetSkinFile();
		CDuiString GetSkinFolder();

		void InitWindow();
		void Notify(TNotifyUI& msg);
		LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	private:
		CDuiString		m_strXMLPath;
		CStdStringPtrMap m_MenuCheckInfo; //保存菜单的单选复选信息
		//std::shared_ptr<Model> mo;
	};
}