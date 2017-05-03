#pragma once
#include"ChakraCore\ChakraCore.h"
#include"EngineRuntime\Component.h"
#include"EngineRuntime\GameObject.h"


class JSComponent :public Component
{
	friend class GameObject;
private:
	virtual void OnInit()override;
	virtual void Update() override;
	virtual Component* Clone() { return nullptr; };

	const std::string m_jsfile;

	static JsRuntimeHandle jsRT;
	static UINT currentSourceContext;
	static JsContextRef jsContex;

public:
	JSComponent();
	virtual ~JSComponent();

	void static Bind();
	void static SetJSFilePath(const std::string& path);
};