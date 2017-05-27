#include "settingarithmetic.h"
#include "ui_settingfloat.h"

RTTR_REGISTRATION
{
RegistrationSetting<float,SettingArithmetic<float>>();
RegistrationSetting<double, SettingArithmetic<double>>();
RegistrationSetting<int, SettingArithmetic<int>>();
RegistrationSetting<unsigned int, SettingArithmetic<unsigned int>>();
}

ArithmeticBase::ArithmeticBase(QWidget *parent, SettingBase* target) :
	SettingWidget(parent),
    ui(new Ui::SettingFloat)
{
    ui->setupUi(this);
}

ArithmeticBase::~ArithmeticBase()
{
    delete ui;
}
