#include "ComponentViewControll.h"
#include "Control\gocontrol.h"

#include "WellPlayMain.h"

using namespace std;

ComponentViewControll::ComponentViewControll(QTabWidget *target):
	parent(target),
	currentSelect(nullptr)
{
	connect(target, &QTabWidget::currentChanged, this, &ComponentViewControll::selectChanged);
	connect(target, &QTabWidget::tabCloseRequested, this, &ComponentViewControll::closeTab);
}

void ComponentViewControll::OnSelect(shared_ptr<GameObject> target)
{
	for (int i=0;i<m_select.size();i++)
	{
		if (m_select[i].lock() == target)
		{
			parent->setCurrentIndex(i);
			return;
		}
	}

	if (m_select.size() >= MaxNumber)
	{
		parent->removeTab(0);
		m_select.pop_front();
	}
	auto lk = Interrupt();
	GOControl* newControl = new GOControl(target);
	parent->addTab(newControl, QString::fromStdString(target->GetName()));
	m_select.push_back(weak_ptr<GameObject>(target));
	lk.unlock();
	parent->setCurrentIndex(m_select.size() - 1);
}

void ComponentViewControll::OnRemoveComView(std::shared_ptr<const GameObject> target)
{
	for (int i = 0; i<m_select.size(); i++)
	{
		if (m_select[i].lock() == target)
		{
			parent->removeTab(i);
			return;
		}
	}
}

void ComponentViewControll::ReadData()
{
	if (currentSelect != nullptr)
		currentSelect->ReadData();
}

void ComponentViewControll::UpdateData()
{
	if (currentSelect != nullptr)
		currentSelect->UpdateData();
}

void ComponentViewControll::selectChanged(int index)
{
	currentSelect = static_cast<GOControl*>(parent->currentWidget());
	assert(currentSelect != nullptr);
}

void ComponentViewControll::closeTab(int index)
{
	m_select.erase(m_select.begin() + index);
	parent->removeTab(index);
}
