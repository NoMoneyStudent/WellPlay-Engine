#pragma once
#include <type_traits>
#include "SettingVar.h"
#include "rttr\registration.h"

class QWidget;

//T必须是QWidget的派生类
template<typename T>
class SettingWrapper
{
public:
	SettingWrapper(QWidget *parent, SettingBase* target)
	{
		static_assert(std::is_base_of<QWidget, T>::value, "T必须是QWidget的派生类");
		new T(parent, target);
	}
	SettingWrapper(const SettingWrapper& r) 
	{ return; }
};

template<typename DataType,typename ControllType>
auto RegistrationSetting()
    ->typename std::enable_if<std::is_base_of<QWidget, ControllType>::value>::type
{
	auto n = rttr::type::get<SettingVar<DataType>>().get_name() + "set";
	rttr::registration::class_<SettingWrapper<ControllType>>(n.data()).constructor<QWidget*, SettingBase*>();
}