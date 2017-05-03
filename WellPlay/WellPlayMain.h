#pragma once
/*
--  此文件仅在编辑器项目中使用，引擎项目不能引用
*/

#include <string>
#include <mutex>
#include "EngineCallBack.h"

void EngineMain(HWND m_hwnd,HWND input);
void ShutDown();
void SetFocus(bool ifFocus);
std::unique_lock<std::mutex> Interrupt();

std::wstring MakeWStr(const std::string& str);
std::string MakeStr(const std::wstring& str);

namespace EngineMessage
{
	void AddMessage(const std::function<void(void)>& message);
}