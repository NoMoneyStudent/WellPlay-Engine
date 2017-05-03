#include "stdafx.h"
#include "ScriptManager.h"
#include <fstream>
#include "EngineRuntime\Component.h"
#include "EngineRuntime\GameObject.h"
#include "boost\variant.hpp"
#include <type_traits>

#pragma comment(lib,"Lua.lib")

using namespace std;
using namespace DirectX;

set<string> ScriptList;

void Lua::AddScript(const std::string & name)
{
	if (ScriptList.find(name) == ScriptList.end())
	{
		ScriptList.insert(name);
		ifstream luafile(name);
		try
		{
			LuaScript::m_lua.executeCode(luafile);
		}
		catch (const std::runtime_error& e)
		{
			//EditorWindows::LogWnd::Print(MakeWStr(e.what()));
		}
	}
}

void Lua::Init()
{
	LuaContext& m_lua = LuaScript::m_lua;

	/*m_lua.registerMember("x", &XMFLOAT2::x);
	m_lua.registerMember("y", &XMFLOAT2::y);*/
	m_lua.registerMember("x", &XMFLOAT3::x);
	m_lua.registerMember("y", &XMFLOAT3::y);
	m_lua.registerMember("z", &XMFLOAT3::z);
	/*m_lua.registerMember("x", &XMFLOAT4::x);
	m_lua.registerMember("y", &XMFLOAT4::y);
	m_lua.registerMember("z", &XMFLOAT4::z);
	m_lua.registerMember("w", &XMFLOAT4::w);*/
	m_lua.writeFunction("_makevector2",
		[](float x, float y) {return shared_ptr<XMFLOAT2>(new XMFLOAT2(x, y)); });
	m_lua.writeFunction("_makevector3",
		[](float x, float y, float z) {return shared_ptr<XMFLOAT3>(new XMFLOAT3(x, y, z)); });
	m_lua.writeFunction("_makevector4",
		[](float x, float y, float z, float w) {return shared_ptr<XMFLOAT4>(new XMFLOAT4(x, y, z, w)); });

	/*m_lua.writeFunction("Log",
		[](const string& data) { EditorWindows::LogWnd::Print(MakeWStr(data)); });*/

#pragma region GameObject
	m_lua.writeVariable("GameObject", LuaContext::EmptyArray);
	m_lua.writeFunction("GameObject","Find", &GameObject::Find);
	m_lua.writeFunction("GameObject","FindAll", &GameObject::FindAll);
	m_lua.writeFunction("GameObject","Instantiate",
		[](boost::variant<const string, shared_ptr<GameObject>> prototype)
	{
		if (prototype.which() == 0)
			return GameObject::Instantiate(boost::get<const string>(prototype));
		else
			return GameObject::Instantiate(boost::get<shared_ptr<GameObject>>(prototype));
	});
	m_lua.writeFunction("GameObject","Destroy",
		[](boost::variant<shared_ptr<Component>, shared_ptr<GameObject>> prototype)
	{
		if (prototype.which() == 0)
			GameObject::Destroy(boost::get<shared_ptr<Component>>(prototype));
		else
			GameObject::Destroy(boost::get<shared_ptr<GameObject>>(prototype));
	});

	m_lua.registerMember<GameObject, weak_ptr<Transform>>(string("transform"),
		[](const GameObject& go) { return go.GetTransform().lock(); });
	m_lua.registerMember<GameObject, bool>(string("activeInHierarchy"),
		[](const GameObject& go) { return go.GetActiveInHierarchy(); });
	m_lua.registerMember<bool(GameObject::*)>("selfActive",
		[](const GameObject& go) { return go.GetSelfActive(); },
		[](GameObject& go, bool active) { go.SetActive(active); });
	m_lua.registerMember<string(GameObject::*)>("name",
		[](const GameObject& go) { return go.GetName(); },
		[](GameObject& go, const string& active) { go.SetName(active); });
#pragma endregion

#pragma region Transform
	m_lua.registerMember<bool(Transform::*)>("enable",
		[](const Transform& tran) {return tran.GetEnable(); },
		[](Transform& tran, bool p) { tran.SetEnable(p); });
	m_lua.registerMember<shared_ptr<GameObject>(Transform::*)>(string("gameobject"),
		[](const Transform& tran) {return tran.gameobject(); });
	m_lua.registerMember<XMFLOAT3(Transform::*)>("position",
		[](const Transform& tran) {XMFLOAT3 p; XMStoreFloat3(&p, tran.GetWorldPosition()); return p; },
		[](Transform& tran, const XMFLOAT3& p) { tran.SetWorldPosition(XMLoadFloat3(&p)); });
	m_lua.registerMember<XMFLOAT4(Transform::*)>("rotation",
		[](const Transform& tran) {XMFLOAT4 r; XMStoreFloat4(&r, tran.GetWorldRotation()); return r; },
		[](Transform& tran, const XMFLOAT4& r) { tran.SetWorldRotation(XMLoadFloat4(&r)); });
	m_lua.registerMember<XMFLOAT3(Transform::*)>("scale",
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetWorldScale()); return s; },
		[](Transform& tran, const XMFLOAT3& s) { tran.SetWorldScale(XMLoadFloat3(&s)); });
	m_lua.registerMember<XMFLOAT3(Transform::*)>("localposition",
		[](const Transform& tran) {XMFLOAT3 p; XMStoreFloat3(&p, tran.GetLocalPosition()); return p; },
		[](Transform& tran, const XMFLOAT3& p) { tran.SetWorldPosition(XMLoadFloat3(&p)); });
	m_lua.registerMember<XMFLOAT4(Transform::*)>("localrotation",
		[](const Transform& tran) {XMFLOAT4 r; XMStoreFloat4(&r, tran.GetLocalRotation()); return r; },
		[](Transform& tran, const XMFLOAT4& r) { tran.SetWorldRotation(XMLoadFloat4(&r)); });
	m_lua.registerMember<XMFLOAT3(Transform::*)>("localscale",
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetLocalScale()); return s; },
		[](Transform& tran, const XMFLOAT3& s) { tran.SetWorldScale(XMLoadFloat3(&s)); });
	m_lua.registerMember<XMFLOAT3(Transform::*)>(string("right"),
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetRight()); return s; });
	m_lua.registerMember<XMFLOAT3(Transform::*)>(string("forward"),
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetForward()); return s; });
	m_lua.registerMember<XMFLOAT3(Transform::*)>(string("up"),
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetUp()); return s; });
	m_lua.registerMember<XMFLOAT3(Transform::*)>(string("down"),
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetDown()); return s; });
	m_lua.registerMember<XMFLOAT3(Transform::*)>(string("behind"),
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetBehind()); return s; });
	m_lua.registerMember<XMFLOAT3(Transform::*)>(string("left"),
		[](const Transform& tran) {XMFLOAT3 s; XMStoreFloat3(&s, tran.GetLeft()); return s; });
	m_lua.registerMember<shared_ptr<Transform>(Transform::*)>("parent",
		[](const Transform& tran) { return tran.GetParent(); },
		[](Transform& tran, shared_ptr<Transform> parent) { tran.SetParent(parent); });
	m_lua.registerMember<vector<shared_ptr<Transform>>(Transform::*)>(string("children"),
		[](const Transform& tran) { return tran.GetChildren(); });
	m_lua.registerFunction(string("AddChildren"), &Transform::AddChild);
#pragma endregion

	AddScript("Script\\Class.lua");
	AddScript("Script\\vector3.lua");
}
