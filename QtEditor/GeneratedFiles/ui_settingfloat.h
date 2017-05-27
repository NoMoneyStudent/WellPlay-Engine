/********************************************************************************
** Form generated from reading UI file 'settingfloat.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGFLOAT_H
#define UI_SETTINGFLOAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingFloat
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;

    void setupUi(QWidget *SettingFloat)
    {
        if (SettingFloat->objectName().isEmpty())
            SettingFloat->setObjectName(QStringLiteral("SettingFloat"));
        SettingFloat->resize(225, 39);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SettingFloat->sizePolicy().hasHeightForWidth());
        SettingFloat->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(SettingFloat);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(SettingFloat);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QFont font;
        font.setPointSize(10);
        lineEdit->setFont(font);
        lineEdit->setAcceptDrops(false);
        lineEdit->setFrame(true);

        verticalLayout->addWidget(lineEdit);


        retranslateUi(SettingFloat);

        QMetaObject::connectSlotsByName(SettingFloat);
    } // setupUi

    void retranslateUi(QWidget *SettingFloat)
    {
        SettingFloat->setWindowTitle(QApplication::translate("SettingFloat", "Form", Q_NULLPTR));
        lineEdit->setInputMask(QString());
    } // retranslateUi

};

namespace Ui {
    class SettingFloat: public Ui_SettingFloat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGFLOAT_H
