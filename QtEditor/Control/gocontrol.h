#ifndef GOCONTROL_H
#define GOCONTROL_H

#include <qwidget.h>
#include "combase.h"
#include <memory>
#include "EngineRuntime\GameObject.h"

namespace Ui {
class GOControl;
}

class GOControl : public QWidget
{
    Q_OBJECT

public:
    explicit GOControl(std::shared_ptr<GameObject> target);
    ~GOControl();

	void ReadData();
	void UpdateData();

private:
	template <class T> void AddComControl(Component* com)
	{
		static_assert(std::is_base_of<ComBase, T>::value, "");
		ComBase* newcontrol = new T(ui->ComponentList->layout());
		newcontrol->SetComponent(com);
		newcontrol->ReadData();
		newcontrol->UpdateData();
		m_control.push_back(newcontrol);
	}

	QString name;
	bool selfactive;
    Ui::GOControl *ui;
	std::weak_ptr<GameObject> m_target;
	std::vector<ComBase*> m_control;
};

#endif // GOCONTROL_H
