/********************************************************************************
** Form generated from reading UI file 'combase.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMBASE_H
#define UI_COMBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ComBase
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *bar;
    QHBoxLayout *horizontalLayout;
    QCheckBox *ComTitle;
    QSpacerItem *horizontalSpacer;
    QPushButton *RemoveButton;

    void setupUi(QFrame *ComBase)
    {
        if (ComBase->objectName().isEmpty())
            ComBase->setObjectName(QStringLiteral("ComBase"));
        ComBase->resize(514, 420);
        ComBase->setFrameShape(QFrame::WinPanel);
        ComBase->setFrameShadow(QFrame::Plain);
        ComBase->setLineWidth(2);
        ComBase->setMidLineWidth(0);
        verticalLayout = new QVBoxLayout(ComBase);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        bar = new QWidget(ComBase);
        bar->setObjectName(QStringLiteral("bar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bar->sizePolicy().hasHeightForWidth());
        bar->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(bar);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ComTitle = new QCheckBox(bar);
        ComTitle->setObjectName(QStringLiteral("ComTitle"));
        ComTitle->setCheckable(true);
        ComTitle->setChecked(false);
        ComTitle->setTristate(false);

        horizontalLayout->addWidget(ComTitle);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        RemoveButton = new QPushButton(bar);
        RemoveButton->setObjectName(QStringLiteral("RemoveButton"));
        RemoveButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon;
        icon.addFile(QStringLiteral(":/qss_icons/rc/close-pressed.png"), QSize(), QIcon::Normal, QIcon::Off);
        RemoveButton->setIcon(icon);
        RemoveButton->setFlat(true);

        horizontalLayout->addWidget(RemoveButton);


        verticalLayout->addWidget(bar, 0, Qt::AlignTop);


        retranslateUi(ComBase);

        RemoveButton->setDefault(false);


        QMetaObject::connectSlotsByName(ComBase);
    } // setupUi

    void retranslateUi(QFrame *ComBase)
    {
        ComBase->setWindowTitle(QApplication::translate("ComBase", "Frame", Q_NULLPTR));
        ComTitle->setText(QApplication::translate("ComBase", "component", Q_NULLPTR));
        RemoveButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ComBase: public Ui_ComBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMBASE_H
