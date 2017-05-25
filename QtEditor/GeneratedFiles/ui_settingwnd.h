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
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingWnd
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;

    void setupUi(QDockWidget *SettingWnd)
    {
        if (SettingWnd->objectName().isEmpty())
            SettingWnd->setObjectName(QStringLiteral("SettingWnd"));
        SettingWnd->resize(450, 392);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(11, -1, -1, -1);
        treeWidget = new QTreeWidget(dockWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->header()->setVisible(false);

        verticalLayout->addWidget(treeWidget);

        SettingWnd->setWidget(dockWidgetContents);

        retranslateUi(SettingWnd);

        QMetaObject::connectSlotsByName(SettingWnd);
    } // setupUi

    void retranslateUi(QDockWidget *SettingWnd)
    {
        SettingWnd->setWindowTitle(QApplication::translate("SettingWnd", "\350\256\276\347\275\256", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("SettingWnd", "1", Q_NULLPTR));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("SettingWnd", "\351\235\231\346\200\201\346\265\213\350\257\225", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("SettingWnd", "\345\221\265\345\221\265", Q_NULLPTR));
        treeWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class SettingWnd: public Ui_SettingWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWND_H
