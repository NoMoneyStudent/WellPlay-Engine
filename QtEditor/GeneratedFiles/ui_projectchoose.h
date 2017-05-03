/********************************************************************************
** Form generated from reading UI file 'projectchoose.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTCHOOSE_H
#define UI_PROJECTCHOOSE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectChoose
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QTabWidget *tabWidget;
    QWidget *Exist;
    QHBoxLayout *horizontalLayout;
    QListWidget *PathList;
    QWidget *New;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *PathLabel;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *ProjectChoose)
    {
        if (ProjectChoose->objectName().isEmpty())
            ProjectChoose->setObjectName(QStringLiteral("ProjectChoose"));
        ProjectChoose->resize(600, 400);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ProjectChoose->sizePolicy().hasHeightForWidth());
        ProjectChoose->setSizePolicy(sizePolicy);
        ProjectChoose->setMinimumSize(QSize(600, 400));
        ProjectChoose->setMaximumSize(QSize(600, 400));
        verticalLayout_2 = new QVBoxLayout(ProjectChoose);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(ProjectChoose);
        label->setObjectName(QStringLiteral("label"));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        tabWidget = new QTabWidget(ProjectChoose);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tabWidget->setTabBarAutoHide(false);
        Exist = new QWidget();
        Exist->setObjectName(QStringLiteral("Exist"));
        horizontalLayout = new QHBoxLayout(Exist);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        PathList = new QListWidget(Exist);
        PathList->setObjectName(QStringLiteral("PathList"));

        horizontalLayout->addWidget(PathList);

        tabWidget->addTab(Exist, QString());
        New = new QWidget();
        New->setObjectName(QStringLiteral("New"));
        verticalLayout_3 = new QVBoxLayout(New);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(New);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        PathLabel = new QLineEdit(New);
        PathLabel->setObjectName(QStringLiteral("PathLabel"));
        PathLabel->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(PathLabel);

        pushButton = new QPushButton(New);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(New);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);
        pushButton_2->setMinimumSize(QSize(200, 50));

        horizontalLayout_3->addWidget(pushButton_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(2, 1);

        verticalLayout_3->addLayout(verticalLayout);

        tabWidget->addTab(New, QString());

        verticalLayout_2->addWidget(tabWidget);


        retranslateUi(ProjectChoose);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ProjectChoose);
    } // setupUi

    void retranslateUi(QDialog *ProjectChoose)
    {
        ProjectChoose->setWindowTitle(QApplication::translate("ProjectChoose", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("ProjectChoose", "<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">\351\200\211\346\213\251\351\241\271\347\233\256</span></p></body></html>", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(Exist), QApplication::translate("ProjectChoose", "\347\216\260\346\234\211\351\241\271\347\233\256", Q_NULLPTR));
        label_2->setText(QApplication::translate("ProjectChoose", "\351\241\271\347\233\256\347\233\256\345\275\225:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ProjectChoose", "...", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ProjectChoose", "\347\241\256\345\256\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(New), QApplication::translate("ProjectChoose", "\346\226\260\345\273\272\351\241\271\347\233\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ProjectChoose: public Ui_ProjectChoose {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTCHOOSE_H
