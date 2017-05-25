#pragma once
#include <qobject.h>
#include <qicon.h>
#include <qdockwidget.h>
#include <qlistwidget.h>
#include "EngineCallBack.h"

class LogViewControll :public QObject
{
	Q_OBJECT
public:
	explicit LogViewControll(QDockWidget *target);
	~LogViewControll();

	void Log(const QString data,int mode);

private :
	QListWidget* m_target;
	QIcon* m_icons;
	unsigned int count = 0;
	static const unsigned int Maxcount = 50;
};