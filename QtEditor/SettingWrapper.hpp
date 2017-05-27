#pragma once
#include <type_traits>
#include "SettingVar.h"
#include "rttr\registration.h"

#include <qwidget.h>

//所有的Setting控件类必须继承
class SettingWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SettingWidget(QWidget *parent) :QWidget(parent) {};
	~SettingWidget() = default;
};

//T必须是SettingWidget的派生类
template<typename T>
class SettingWrapper
{
public:
	SettingWrapper(QWidget *parent, SettingBase* target)
	{
		static_assert(std::is_base_of<SettingWidget, T>::value, "T必须是SettingWidget的派生类");
		new T(parent, target);
	}
	SettingWrapper(const SettingWrapper& r) 
	{ return; }
};

template<typename DataType,typename ControllType>
auto RegistrationSetting()
    ->typename std::enable_if<std::is_base_of<SettingWidget, ControllType>::value>::type
{
	auto n = rttr::type::get<SettingVar<DataType>>().get_name() + "set";
	rttr::registration::class_<SettingWrapper<ControllType>>(n.data()).constructor<QWidget*, SettingBase*>();
}