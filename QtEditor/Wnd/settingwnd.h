#ifndef SETTINGWND_H
#define SETTINGWND_H

#include <QDockWidget>
#include <qstyleditemdelegate.h>
#include <qstandarditemmodel.h>
#include "SettingVar.h"

Q_DECLARE_METATYPE(SettingBase*)

namespace Ui {
class SettingWnd;
}

class SettingModel :public QStandardItemModel
{
	Q_OBJECT
public:
	SettingModel() = default;
	~SettingModel() = default;

	void InitSetting();

private:
	void AddSettingItem(const std::vector<SettingBase*>& items);
};

class SettingDelegate :public QStyledItemDelegate
{
	Q_OBJECT
public:
	SettingDelegate() = default;
	~SettingDelegate() = default;

	virtual QWidget *createEditor(QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};

class SettingWnd : public QDockWidget
{
    Q_OBJECT

public:
    explicit SettingWnd(QWidget *parent = 0);
    ~SettingWnd();

private:
    Ui::SettingWnd *ui;
	SettingDelegate* m_delegate;
	SettingModel* m_model;
};

#endif // SETTINGWND_H
