#include "LogViewControll.h"
#include <assert.h>
#include "EditorUtility.h"

LogViewControll::LogViewControll(QDockWidget *target):
	m_icons(new QIcon[LogMode::ModeNum])
{
	m_target = target->findChild<QListWidget*>("DebugLog");
	assert(m_target != nullptr);

	m_icons[0].addFile(QStringLiteral(":/qss_icons/rc/undock.png"));
	m_icons[1].addFile(QStringLiteral(":/qss_icons/rc/checkbox_checked_focus.png"));
	m_icons[2].addFile(QStringLiteral(":/qss_icons/rc/close-pressed.png"));

	Utility::currentLog = this;
}

LogViewControll::~LogViewControll()
{
	delete[] m_icons;

	Utility::currentLog = nullptr;
}

void LogViewControll::Log(const QString data, int mode)
{
	count++;
	if (count > Maxcount)
	{
		count = Maxcount;
		m_target->removeItemWidget(m_target->item(0));
	}
	QListWidgetItem *qlistwidgetitem = new QListWidgetItem(m_target);
	qlistwidgetitem->setIcon(m_icons[mode]);
	qlistwidgetitem->setText(data);
}