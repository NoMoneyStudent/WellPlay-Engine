#ifndef SETTINGWND_H
#define SETTINGWND_H

#include <QDockWidget>

namespace Ui {
class SettingWnd;
}

class SettingWnd : public QDockWidget
{
    Q_OBJECT

public:
    explicit SettingWnd(QWidget *parent = 0);
    ~SettingWnd();

private:
    Ui::SettingWnd *ui;
};

#endif // SETTINGWND_H
