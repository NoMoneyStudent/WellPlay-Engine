#pragma once
#include <luawrapper\LuaContext.hpp>
#include "LuaScript.h"

#pragma comment(lib,"Lua.lib")

namespace Lua
{
	void AddScript(const std::string& name);
	void Init();
}