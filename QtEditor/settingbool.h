#pragma once
#include <qcheckbox.h>
#include <qboxlayout.h>
#include "SettingVar.h"
#include "SettingWrapper.hpp"

class SettingBool : public SettingWidget
{
	Q_OBJECT
public:
	explicit SettingBool(QWidget *parent, SettingBase* target) :
		SettingWidget(parent),
		m_target(static_cast<SettingVar<bool>*>(target))
	{
		auto verticalLayout = new QVBoxLayout(this);
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		m_check = new QCheckBox();
		m_check->setChecked(*m_target);
		verticalLayout->addWidget(m_check);
	}
	~SettingBool() = default;
	virtual void FinishEdit()
	{
		*m_target = m_check->isChecked();
	}
private:
	SettingVar<bool>* m_target;
	QCheckBox* m_check;
};