#include "stdafx.h"
#include "JSComponent.h"
#include "Utility\FileUtility.h"

#pragma comment(lib,"ChakraCore.lib")

using namespace std;

UINT JSComponent::currentSourceContext = 0;
JsRuntimeHandle JSComponent::jsRT;
JsContextRef JSComponent::jsContex;

void JSComponent::OnInit()
{
}

void JSComponent::Update()
{

}

JSComponent::JSComponent()
{
}

JSComponent::~JSComponent()
{
}

void setProperty(JsValueRef object, const wchar_t *propertyName, JsValueRef property)
{
	JsPropertyIdRef propertyId;
	JsGetPropertyIdFromName(propertyName, &propertyId);
	JsSetProperty(object, propertyId, property, true);
}

void setCallback(JsValueRef object, const wchar_t *propertyName, JsNativeFunction callback, void *callbackState)
{
	JsPropertyIdRef propertyId;
	JsGetPropertyIdFromName(propertyName, &propertyId);
	JsValueRef function;
	JsCreateFunction(callback, callbackState, &function);
	JsSetProperty(object, propertyId, function, true);
}

JsValueRef CALLBACK JSLog(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	for (unsigned int index = 0; index < argumentCount; index++)
	{
		if (index > 1)
		{
			wprintf(L" ");
		}
		JsValueRef stringValue;
		JsValueType type;
		JsGetValueType(arguments[index], &type);
		if (JsConvertValueToString(arguments[index], &stringValue)==JsNoError)
		{
			const wchar_t *string;
			size_t length;
			JsStringToPointer(stringValue, &string, &length);
			std::wstring f(string);
			//EditorWindows::LogWnd::Print(f);
		}
		else
		{
			//EditorWindows::LogWnd::Print(L"不能转化 ");
		}
	}
	return JS_INVALID_REFERENCE;
}

void JSComponent::SetJSFilePath(const std::string & path)
{	
	/*try
	{*/
		JsValueRef result;
		WcharArray script = FileUtility::ReadFileWchar(path);

		if (JsRunScript(script.data(), currentSourceContext++, L"", &result) != JsNoError)
		{
			// Get error message
			JsValueRef exception;
			ASSERT_SUCCEEDED(JsGetAndClearException(&exception), "failed to get and clear exception");
			
			JsPropertyIdRef messageName;
			ASSERT_SUCCEEDED(JsGetPropertyIdFromName(L"message", &messageName), "failed to get error message id");

			JsValueRef messageValue;
			ASSERT_SUCCEEDED(JsGetProperty(exception, messageName, &messageValue), "failed to get error message");

			const wchar_t *message;
			size_t length;
			ASSERT_SUCCEEDED(JsStringToPointer(messageValue, &message, &length), "failed to convert error message");
		
			//EditorWindows::LogWnd::Print(message);
			return;
		}

		JsValueRef stringResult;
		const wchar_t *returnValue;
		size_t stringLength;
		ASSERT_SUCCEEDED(JsConvertValueToString(result, &stringResult), "failed to convert value to string.");
		
		ASSERT_SUCCEEDED(JsStringToPointer(stringResult, &returnValue, &stringLength), "failed to convert return value.");
		
		//EditorWindows::LogWnd::Print(returnValue);
		return;
	/*}
	catch (...)
	{
		EditorWindows::LogWnd::Print(L"chakrahost: fatal error: internal error.\n");
	}*/
}

void JSComponent::Bind()
{
	ASSERT_SUCCEEDED(JsCreateRuntime(JsRuntimeAttributeEnableExperimentalFeatures, nullptr, &jsRT), "");
	ASSERT_SUCCEEDED(JsCreateContext(jsRT, &jsContex), "");
	ASSERT_SUCCEEDED(JsSetCurrentContext(jsContex), "");

	JsValueRef globalObject;
	JsGetGlobalObject(&globalObject);
	JsValueRef console;
	JsCreateObject(&console);
	setProperty(globalObject, L"console", console);
	setCallback(console, L"log", JSLog, nullptr);
}