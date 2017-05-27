#pragma once
#include <qlineedit.h>
#include <qboxlayout.h>
#include "SettingVar.h"
#include "SettingWrapper.hpp"

class SettingString : public SettingWidget
{
	Q_OBJECT
public:
	explicit SettingString(QWidget *parent, SettingBase* target) :
		SettingWidget(parent),
		m_target(static_cast<SettingVar<std::wstring>*>(target))
	{
		auto verticalLayout = new QVBoxLayout(this);
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		m_line = new QLineEdit();
		m_line->setText(QString::fromStdWString(*m_target));
		verticalLayout->addWidget(m_line);
		auto f = m_line->font();
		f.setPointSize(10);
		m_line->setFont(f);
	}
	~SettingString() = default;
	virtual void FinishEdit()
	{
		*m_target = m_line->text().toStdWString();
	}
private:
	SettingVar<std::wstring>* m_target;
	QLineEdit* m_line;
};