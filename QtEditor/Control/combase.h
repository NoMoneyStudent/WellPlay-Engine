#ifndef COMBASE_H
#define COMBASE_H

#include <QFrame>
#include <assert.h>
#include "EngineRuntime\Component.h"

namespace Ui {
class ComBase;
}

class ComBase : public QFrame
{
    Q_OBJECT

public:
    explicit ComBase(QLayout *parent);
    virtual ~ComBase();

	void SetRemoveButton(bool able);
	void SetComEnable(bool able);
	void SetComWidget(QWidget* component);
	void SetComName(const QString& name);
	void SetComCheckable(bool able);

	virtual void ReadData() = 0;
	virtual void UpdateData() = 0;
	virtual void SetComponent(Component* target) = 0;

private:
    Ui::ComBase *ui;
};

#endif // COMBASE_H
