#pragma once
#include <functional>
#include <vector>
#include <string>
#include <memory>
#include "EngineRuntime\GameObject.h"

namespace EngineCallBack
{
	extern std::function<void(const std::wstring&)> OnLog;
	extern std::function<void(const GameObject&)> OnAddGameObject;
	extern std::function<void(const GameObject&)> OnRemoveGameObject;
	/*   1-Parent   2-Child  3-index  */
	extern std::function<void(const GameObject&, const GameObject&, int)> OnMoveGameObject;
	extern std::function<void(void)> OnLoadedScene;
	extern std::function<void(void)> OnFinishUpdate;
}