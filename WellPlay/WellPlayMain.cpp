//  定义应用程序的入口点。
//
#include "stdafx.h"
#include "wnd\Editor.h"
#include "GameInput.h"
#include "SystemTime.h"
#include "Render\GraphicsCore.h"
#include "RenderCore.h"
#include "EngineRuntime\Scene.h"
#include "Script\ScriptManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Editor m_Editor(hInstance);

	RenderCore::Initialize(m_Editor.GetDXWndHwnd());
	GameInput::Initialize(m_Editor.GetDXWndHwnd());
	SystemTime::Initialize();
	Lua::Init();
	MSG msg = { 0 };
	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!CPaintManagerUI::TranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		if (msg.message == WM_QUIT)
			break;

		SystemTime::Update();
		GameInput::Update();
		Scene::GetCurrentScene()->Update();

		RenderCore::Update();
		RenderCore::Render();
	}

	GameInput::Shutdown();
	RenderCore::Shutdown();

	return 0;
}