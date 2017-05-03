#include "stdafx.h"
#include "GameInput.h"
#include "SystemTime.h"
#include "RenderCore.h"
#include "EngineRuntime\Scene.h"
#include "Script\ScriptManager.h"
#include "Resource\ModelImport.h"
#include "WellPlayMain.h"

#include <atomic>
#include <mutex>
#include <queue>

using namespace std;

atomic<bool> ifShutDown = false;
mutex InterruptLock;

namespace EngineCallBack
{
	function<void(const std::wstring&)> OnLog;
	function<void(const GameObject&)> OnAddGameObject;
	function<void(const GameObject&)> OnRemoveGameObject;
	std::function<void(const GameObject&, const GameObject&,int)> OnMoveGameObject;
	function<void(void)> OnLoadedScene;
	function<void(void)> OnFinishUpdate;
}
namespace EngineMessage
{
	mutex m_lock;
	queue<function<void(void)>> MessageQueue;

	void AddMessage(const function<void(void)>& message)
	{
		lock_guard<mutex> lk(m_lock);
		MessageQueue.push(std::move(message));
	}

	vector<function<void(void)>> PopMessage()
	{
		vector<function<void(void)>> res;
		lock_guard<mutex> lk(m_lock);
		while (!MessageQueue.empty())
		{
			res.push_back(MessageQueue.front());
			MessageQueue.pop();
		}
		return res;
	}
}

void ShutDown()
{
	ifShutDown = true;
}
unique_lock<mutex> Interrupt()
{
	return unique_lock<mutex>(InterruptLock);
}
void SetFocus(bool Focus)
{
	GameInput::ifFocus = Focus;
}

void EngineMain(HWND m_hwnd,HWND input)
{
	RenderCore::Initialize(m_hwnd);
	GameInput::Initialize(input);
	SystemTime::Initialize();
	//Lua::Init();
	ModelImport::ImportModel("E:/test.FBX");

	EngineCallBack::OnLog(L"引擎初始化完成");

	while (!ifShutDown)
	{
		SystemTime::Update();
		GameInput::Update();
		
		{
			lock_guard<mutex> lk(InterruptLock);
			Scene::GetCurrentScene()->Update();
		}

		RenderCore::Update();
		RenderCore::Render();

		auto messages = EngineMessage::PopMessage();
		for (auto& message : messages)
		{
			message();
		}
	}

	GameInput::Shutdown();
	RenderCore::Shutdown();
}