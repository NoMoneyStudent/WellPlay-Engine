#ifndef SETTINGFLOAT_H
#define SETTINGFLOAT_H

#include <assert.h>
#include <QWidget>
#include "SettingVar.h"

namespace Ui {
class SettingFloat;
}

class SettingFloat : public QWidget
{
    Q_OBJECT
public:
    explicit SettingFloat(QWidget *parent,SettingBase* target);
    ~SettingFloat();

private:
    Ui::SettingFloat *ui;
};
#endif // SETTINGFLOAT_H