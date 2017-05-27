/********************************************************************************
** Form generated from reading UI file 'settingwnd.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGWND_H
#define UI_SETTINGWND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingWnd
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;

    void setupUi(QDockWidget *SettingWnd)
    {
        if (SettingWnd->objectName().isEmpty())
            SettingWnd->setObjectName(QStringLiteral("SettingWnd"));
        SettingWnd->resize(450, 392);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        treeView = new QTreeView(dockWidgetContents);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setFrameShape(QFrame::NoFrame);
        treeView->setFrameShadow(QFrame::Plain);
        treeView->setProperty("showDropIndicator", QVariant(false));
        treeView->setUniformRowHeights(true);

        verticalLayout->addWidget(treeView);

        SettingWnd->setWidget(dockWidgetContents);

        retranslateUi(SettingWnd);

        QMetaObject::connectSlotsByName(SettingWnd);
    } // setupUi

    void retranslateUi(QDockWidget *SettingWnd)
    {
        SettingWnd->setWindowTitle(QApplication::translate("SettingWnd", "\350\256\276\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SettingWnd: public Ui_SettingWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWND_H
