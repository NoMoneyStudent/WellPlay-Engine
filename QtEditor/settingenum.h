#pragma once
#include <qcombobox.h>
#include <qboxlayout.h>
#include "SettingVar.h"
#include "SettingWrapper.hpp"

class SettingEnum : public SettingWidget
{
	Q_OBJECT
public:
	explicit SettingEnum(QWidget *parent, SettingBase* target) :
		SettingWidget(parent),
		m_target(static_cast<SettingVar<EnumType>*>(target))
	{
		auto verticalLayout = new QVBoxLayout(this);
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		m_combo = new QComboBox();
		const auto& stdlist = m_target->GetList();
		QStringList list;
		for (auto& item : stdlist)
		{
			list.append(QString::fromStdWString(item));
		}
		m_combo->addItems(list);
		verticalLayout->addWidget(m_combo);
		m_combo->setCurrentIndex(static_cast<int>(m_target->GetData()));
		auto f = m_combo->font();
		f.setPointSize(10);
		m_combo->setFont(f);
	}
	~SettingEnum() = default;
	virtual void FinishEdit()
	{
		m_target->SetData(static_cast<EnumType::type>(m_combo->currentIndex()));
	}
private:
	SettingVar<EnumType>* m_target;
	QComboBox* m_combo;
};