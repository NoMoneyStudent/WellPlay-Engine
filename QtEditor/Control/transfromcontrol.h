#ifndef TRANSFROMCONTROL_H
#define TRANSFROMCONTROL_H

#include "combase.h"
#include "EngineRuntime\Transform.h"

namespace Ui {
class TransfromControl;
}

class TransfromControl : public ComBase
{
    Q_OBJECT

public:
    explicit TransfromControl(QLayout *parent);
    ~TransfromControl();

	virtual void ReadData() override;
	virtual void UpdateData() override;
	virtual void SetComponent(Component* target) override;

private:
    Ui::TransfromControl *ui;
	Transform* m_target;
	XMFLOAT3 p, r, s;
};

#endif // TRANSFROMCONTROL_H
