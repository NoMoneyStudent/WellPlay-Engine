/********************************************************************************
** Form generated from reading UI file 'gocontrol.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOCONTROL_H
#define UI_GOCONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GOControl
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *ComponentList;
    QVBoxLayout *verticalLayout;
    QWidget *GameObjectBar;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *GOEnable;
    QLineEdit *GOName;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *GOControl)
    {
        if (GOControl->objectName().isEmpty())
            GOControl->setObjectName(QStringLiteral("GOControl"));
        GOControl->resize(274, 501);
        horizontalLayout = new QHBoxLayout(GOControl);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(GOControl);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        ComponentList = new QWidget();
        ComponentList->setObjectName(QStringLiteral("ComponentList"));
        ComponentList->setGeometry(QRect(0, 0, 272, 499));
        verticalLayout = new QVBoxLayout(ComponentList);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        GameObjectBar = new QWidget(ComponentList);
        GameObjectBar->setObjectName(QStringLiteral("GameObjectBar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GameObjectBar->sizePolicy().hasHeightForWidth());
        GameObjectBar->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(GameObjectBar);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        GOEnable = new QCheckBox(GameObjectBar);
        GOEnable->setObjectName(QStringLiteral("GOEnable"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(GOEnable->sizePolicy().hasHeightForWidth());
        GOEnable->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(GOEnable, 0, Qt::AlignLeft);

        GOName = new QLineEdit(GameObjectBar);
        GOName->setObjectName(QStringLiteral("GOName"));
        sizePolicy.setHeightForWidth(GOName->sizePolicy().hasHeightForWidth());
        GOName->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(GOName, 0, Qt::AlignLeft);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addWidget(GameObjectBar, 0, Qt::AlignTop);

        scrollArea->setWidget(ComponentList);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(GOControl);

        QMetaObject::connectSlotsByName(GOControl);
    } // setupUi

    void retranslateUi(QWidget *GOControl)
    {
        GOControl->setWindowTitle(QApplication::translate("GOControl", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GOControl: public Ui_GOControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOCONTROL_H
