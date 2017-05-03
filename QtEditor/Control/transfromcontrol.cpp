#include "transfromcontrol.h"
#include "ui_transfromcontrol.h"
#include <DirectXMath.h>

TransfromControl::TransfromControl(QLayout *parent) :
    ComBase(parent),
    ui(new Ui::TransfromControl)
{
	QWidget* control = new QWidget;
    ui->setupUi(control);
	ComBase::SetComWidget(control);
	ComBase::SetComName("Transfrom");
	ComBase::SetRemoveButton(false);
	ComBase::SetComEnable(true);
	ComBase::SetComCheckable(false);
}

TransfromControl::~TransfromControl()
{
    delete ui;
}

void TransfromControl::ReadData()
{
	XMStoreFloat3(&p, m_target->GetLocalPosition());
	r = m_target->GetLocalEulerAngles();
	XMStoreFloat3(&s, m_target->GetLocalScale());
}

void TransfromControl::UpdateData()
{
	ui->px->setText(QString::number(p.x));
	ui->px->setCursorPosition(0);
	ui->py->setText(QString::number(p.y));
	ui->py->setCursorPosition(0);
	ui->pz->setText(QString::number(p.z));
	ui->pz->setCursorPosition(0);
	ui->rx->setText(QString::number(r.x));
	ui->rx->setCursorPosition(0);
	ui->ry->setText(QString::number(r.y));
	ui->ry->setCursorPosition(0);
	ui->rz->setText(QString::number(r.z));
	ui->rz->setCursorPosition(0);
	ui->sx->setText(QString::number(s.x));
	ui->sx->setCursorPosition(0);
	ui->sy->setText(QString::number(s.y));
	ui->sy->setCursorPosition(0);
	ui->sz->setText(QString::number(s.z));
	ui->sz->setCursorPosition(0);
}

void TransfromControl::SetComponent(Component * target)
{
	m_target = static_cast<Transform*>(target);
	assert(m_target != nullptr);
}
