#include "settingfloat.h"
#include "ui_settingfloat.h"
#include "SettingWrapper.hpp"

RTTR_REGISTRATION
{
	RegistrationSetting<float,SettingFloat>();
}

SettingFloat::SettingFloat(QWidget *parent, SettingBase* target) :
    QWidget(parent),
    ui(new Ui::SettingFloat)
{
    ui->setupUi(this);
}

SettingFloat::~SettingFloat()
{
    delete ui;
}
