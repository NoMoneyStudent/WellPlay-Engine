#pragma once
#include"Script\ScriptManager.h"
#include "EngineRuntime\Component.h"

class LuaScript:public Component
{
	friend class GameObject;
private:
	virtual void OnInit()override;
	virtual void Update() override;
	virtual Component* Clone() { return nullptr; };

public:
	LuaScript();
	virtual ~LuaScript();

	static LuaContext m_lua;
	const std::string m_luafile;

	static void SetLuaFilePath(const std::string& path);
};