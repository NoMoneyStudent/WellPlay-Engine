#pragma once
#include <qobject.h>
#include <qtabwidget.h>
#include <deque>
#include "Control\gocontrol.h"
#include "EngineRuntime\GameObject.h"

class ComponentViewControll :public QObject
{
	Q_OBJECT

public:
	explicit ComponentViewControll(QTabWidget *target);
	~ComponentViewControll() = default;

	void OnSelect(std::shared_ptr<GameObject> target);
	void OnRemoveComView(std::shared_ptr<const GameObject> target);
	void ReadData();
	void UpdateData();

private:
	void selectChanged(int index);
	void closeTab(int index);

private:
	QTabWidget* parent;
	std::deque<std::weak_ptr<GameObject>> m_select;

	GOControl* currentSelect;

	static const unsigned int MaxNumber = 10;
};