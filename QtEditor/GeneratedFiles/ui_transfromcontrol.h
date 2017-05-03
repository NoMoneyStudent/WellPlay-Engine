/********************************************************************************
** Form generated from reading UI file 'transfromcontrol.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSFROMCONTROL_H
#define UI_TRANSFROMCONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransfromControl
{
public:
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_4;
    QLineEdit *sx;
    QLineEdit *sy;
    QLabel *label_3;
    QLineEdit *sz;
    QLineEdit *ry;
    QLineEdit *rx;
    QLabel *label_6;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *rz;
    QLineEdit *px;
    QLineEdit *pz;
    QLineEdit *py;

    void setupUi(QWidget *TransfromControl)
    {
        if (TransfromControl->objectName().isEmpty())
            TransfromControl->setObjectName(QStringLiteral("TransfromControl"));
        TransfromControl->resize(479, 176);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TransfromControl->sizePolicy().hasHeightForWidth());
        TransfromControl->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(TransfromControl);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(5);
        gridLayout->setVerticalSpacing(0);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        label_5 = new QLabel(TransfromControl);
        label_5->setObjectName(QStringLiteral("label_5"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        label_4 = new QLabel(TransfromControl);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 0, 1, 1, 1);

        sx = new QLineEdit(TransfromControl);
        sx->setObjectName(QStringLiteral("sx"));

        gridLayout->addWidget(sx, 4, 1, 1, 1);

        sy = new QLineEdit(TransfromControl);
        sy->setObjectName(QStringLiteral("sy"));

        gridLayout->addWidget(sy, 4, 2, 1, 2);

        label_3 = new QLabel(TransfromControl);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        sz = new QLineEdit(TransfromControl);
        sz->setObjectName(QStringLiteral("sz"));

        gridLayout->addWidget(sz, 4, 4, 1, 1);

        ry = new QLineEdit(TransfromControl);
        ry->setObjectName(QStringLiteral("ry"));

        gridLayout->addWidget(ry, 3, 2, 1, 2);

        rx = new QLineEdit(TransfromControl);
        rx->setObjectName(QStringLiteral("rx"));

        gridLayout->addWidget(rx, 3, 1, 1, 1);

        label_6 = new QLabel(TransfromControl);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 0, 3, 1, 2);

        label = new QLabel(TransfromControl);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        label_2 = new QLabel(TransfromControl);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        rz = new QLineEdit(TransfromControl);
        rz->setObjectName(QStringLiteral("rz"));

        gridLayout->addWidget(rz, 3, 4, 1, 1);

        px = new QLineEdit(TransfromControl);
        px->setObjectName(QStringLiteral("px"));

        gridLayout->addWidget(px, 2, 1, 1, 1);

        pz = new QLineEdit(TransfromControl);
        pz->setObjectName(QStringLiteral("pz"));

        gridLayout->addWidget(pz, 2, 4, 1, 1);

        py = new QLineEdit(TransfromControl);
        py->setObjectName(QStringLiteral("py"));

        gridLayout->addWidget(py, 2, 2, 1, 2);


        retranslateUi(TransfromControl);

        QMetaObject::connectSlotsByName(TransfromControl);
    } // setupUi

    void retranslateUi(QWidget *TransfromControl)
    {
        TransfromControl->setWindowTitle(QApplication::translate("TransfromControl", "Form", Q_NULLPTR));
        label_5->setText(QApplication::translate("TransfromControl", "Y", Q_NULLPTR));
        label_4->setText(QApplication::translate("TransfromControl", "X", Q_NULLPTR));
        label_3->setText(QApplication::translate("TransfromControl", "Scale", Q_NULLPTR));
        label_6->setText(QApplication::translate("TransfromControl", "Z", Q_NULLPTR));
        label->setText(QApplication::translate("TransfromControl", "Position", Q_NULLPTR));
        label_2->setText(QApplication::translate("TransfromControl", "Rotation", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TransfromControl: public Ui_TransfromControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSFROMCONTROL_H
