#include "gocontrol.h"
#include "ui_gocontrol.h"
#include <qstring.h>
#include "EngineRuntime\Transform.h"
#include "Control\transfromcontrol.h"

GOControl::GOControl(std::shared_ptr<GameObject> target) :
    ui(new Ui::GOControl),
	m_target(target)
{
	ui->setupUi(this);
	ui->GOEnable->setChecked(target->GetSelfActive());
	ui->GOName->setText(QString::fromStdString(target->GetName()));

	AddComControl<TransfromControl>(target->GetTransform().lock().get());
	auto components = target->GetAllComponents();
	for (auto& item : components)
	{
		//auto ctype = item.first;
		auto com = item.get();
		if (typeid(*com) == typeid(Transform))
			AddComControl<TransfromControl>(com);
	}
	static_cast<QVBoxLayout*>(ui->ComponentList->layout())->addStretch();
}

GOControl::~GOControl()
{
    delete ui;
}

void GOControl::ReadData()
{
	selfactive = m_target.lock()->GetSelfActive();
	name = QString::fromStdString(m_target.lock()->GetName());

	for (auto& item : m_control)
	{
		item->ReadData();
	}
}

void GOControl::UpdateData()
{
	ui->GOEnable->setChecked(selfactive);
	ui->GOName->setText(name);

	for (auto& item : m_control)
	{
		item->UpdateData();
	}
}
