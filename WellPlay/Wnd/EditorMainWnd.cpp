#include "stdafx.h"
#include "EditorMainWnd.h"
#include "Utility\FileUtility.h"
#include "Resource\FBXImport.h"
#include "RenderCore.h"
#include "Resource\TextureManager.h"
#include "AnimationCore.h"
#include "Render\EngineTuning.h"
#include "resource.h"
#include "EngineRuntime\Scene.h"

using namespace EditorWindows;
using namespace std;
using namespace RenderCore;

EditorMainWnd::EditorMainWnd( LPCTSTR pszXMLPath )
	:  m_strXMLPath(pszXMLPath)
{
	EngineTuning::Initialize();
}

EditorMainWnd::~EditorMainWnd()
{
}

LPCTSTR EditorMainWnd::GetWindowClassName() const
{
	return _T("EditorMain");
}

CDuiString EditorMainWnd::GetSkinFile()
{
	return m_strXMLPath;
}

CDuiString EditorMainWnd::GetSkinFolder()
{
	return _T("Skin");
}

void EditorMainWnd::InitWindow()
{
	SetIcon(IDI_WELLPLAY);
}

void EditorMainWnd::Notify( TNotifyUI& msg )
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("Setting"))
		{
			POINT point;
			GetCursorPos(&point);
			point.x += 5;
			point.y -= 5;
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("EngineVarMenu.xml"), point, &m_pm, &m_MenuCheckInfo, eMenuAlignment_Right);
			ASSERT(pMenu != nullptr, "创建选项菜单失败");
			CMenuUI* rootMenu = pMenu->GetMenuUI();
			if (rootMenu != NULL)
			{
				CMenuElementUI* pNew = new CMenuElementUI;
				pNew->SetName(_T("Menu_Dynamic"));
				pNew->SetText(_T("引擎变量设置"));
				pNew->SetShowExplandIcon(true);
			    EngineTuning::Display(pNew);
				rootMenu->Add(pNew);
			}

			pMenu->ResizeMenu();
		}
		else if (msg.pSender->GetName() == _T("File"))
		{
			POINT point;
			GetCursorPos(&point);
			point.x += 5;
			point.y -= 5;
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("FileMenu.xml"), point, &m_pm, &m_MenuCheckInfo, eMenuAlignment_Left);
			ASSERT(pMenu != nullptr, "创建选项菜单失败");
		}
	}
	__super::Notify(msg);
}

LRESULT EditorMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		BOOL bChecked = pMenuCmd->bChecked;
		CDuiString strMenuName = pMenuCmd->szName;
		CDuiString sUserData = pMenuCmd->szUserData;
		CDuiString sText = pMenuCmd->szText;
		m_pm.DeletePtr(pMenuCmd);

		if (strMenuName == _T("OpenFile"))
		{
			wstring filepath = FileUtility::OpenFile();
			string ex = FileUtility::GetExtension(filepath);
			if (ex == "fbx")
			{
				 FBXImport::ImportModel(string(filepath.begin(), filepath.end()));
				//RenderObject object;
				//object.Create(mo->meshs[mo->meshs.size() - 1], mo->bones, 0);
				//renderQueue.push_back(std::move(object));
				//InitAni(renderQueue[0]);
			}
			else if (ex == "tga")
			{
				const ManagedTexture* t = TextureManager::LoadFromFile(filepath, true);
				isTexture = true;
				texture = t->GetSRV();
			}
		}
	}
	else if (uMsg == WM_CLOSE) 
	{
		PostQuitMessage(0);
	}

	bHandled = false;
	return 0;
}