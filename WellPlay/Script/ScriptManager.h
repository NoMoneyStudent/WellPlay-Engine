#pragma once
#include <luawrapper\LuaContext.hpp>
#include "LuaScript.h"

namespace Lua
{
	void AddScript(const std::string& name);
	void Init();
}