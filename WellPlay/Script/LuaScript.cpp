#include "stdafx.h"
#include "LuaScript.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

LuaContext LuaScript::m_lua;

void LuaScript::OnInit()
{
}

void LuaScript::Update()
{

}

LuaScript::LuaScript()
{
}

LuaScript::~LuaScript()
{
}

void LuaScript::SetLuaFilePath(const std::string & path)
{
	Lua::AddScript(path);
}