#ifndef SETTINGFLOAT_H
#define SETTINGFLOAT_H

#include <assert.h>
#include <QWidget>
#include "SettingVar.h"
#include "SettingWrapper.hpp"
#include <qvalidator.h>

namespace Ui {
class SettingFloat;
}

class ArithmeticBase : public SettingWidget
{
    Q_OBJECT
public:
    explicit ArithmeticBase(QWidget *parent,SettingBase* target);
	~ArithmeticBase();

protected:
    Ui::SettingFloat *ui;
};

template<typename T, typename Enable=void>
class SettingArithmetic : public ArithmeticBase
{
private:
	SettingArithmetic(QWidget *parent, SettingBase* target);
};

template<typename T>
class SettingArithmetic <T, typename std::enable_if<std::is_floating_point<T>::value>::type>
	: public ArithmeticBase
{
public:
	SettingArithmetic(QWidget *parent, SettingBase* target) :
		ArithmeticBase(parent, target) 
	{
		m_target = static_cast<SettingVar<T>*>(target);
		double min = static_cast<double>(m_target->GetMin());
		double max = static_cast<double>(m_target->GetMax());
		//默认显示6位小数
		ui->lineEdit->setValidator(new QDoubleValidator(min, max,6, this));
	};
	~SettingArithmetic() = default;

private:
	SettingVar<T>* m_target;
};

template<typename T>
class SettingArithmetic <T, typename std::enable_if<std::is_integral<T>::value>::type>
	: public ArithmeticBase
{
public:
	SettingArithmetic(QWidget *parent, SettingBase* target) :
		ArithmeticBase(parent, target)
	{
		m_target = static_cast<SettingVar<T>*>(target);
		int min = static_cast<int>(m_target->GetMin());
		int max = static_cast<int>(m_target->GetMax());
		ui->lineEdit->setValidator(new QIntValidator(min, max, this));
	};
	~SettingArithmetic() = default;

private:
	SettingVar<T>* m_target;
};

#endif // SETTINGFLOAT_H