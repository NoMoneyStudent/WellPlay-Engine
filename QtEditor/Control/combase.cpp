#include "combase.h"
#include "ui_combase.h"

ComBase::ComBase(QLayout *parent) :
    QFrame(),
    ui(new Ui::ComBase)
{
    ui->setupUi(this);
	auto p = static_cast<QVBoxLayout*>(parent);
	assert(p != nullptr);
	p->addWidget(this);
}

ComBase::~ComBase()
{
    delete ui;
}

void ComBase::SetRemoveButton(bool able)
{
	ui->RemoveButton->setEnabled(able);
}

void ComBase::SetComEnable(bool able)
{
	ui->ComTitle->setChecked(able);
}

void ComBase::SetComWidget(QWidget * component)
{
	ui->verticalLayout->addWidget(component,0, Qt::Alignment(Qt::AlignTop));
}

void ComBase::SetComName(const QString& name)
{
	ui->ComTitle->setText(name);
}

void ComBase::SetComCheckable(bool able)
{
	ui->ComTitle->setCheckable(able);
}
