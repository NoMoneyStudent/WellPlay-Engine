#include "stdafx.h"
#include "SettingVar.h"

using namespace std;

vector<SettingBase*>& settingList()
{
	//为什么要这样写？为了控制全局变量的初始化顺序
	static vector<SettingBase*> _settinglist;
	return _settinglist;
}

const vector<SettingBase*>& GetSettingList()
{
	return settingList();
}

SettingBase::SettingBase(const std::wstring & path, const ChangedCallBack & callback):
	m_path(path),
	m_OnChanged(callback)
{
	settingList().push_back(this);
}