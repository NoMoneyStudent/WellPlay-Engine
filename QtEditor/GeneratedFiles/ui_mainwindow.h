/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *Run;
    QAction *Next;
    QAction *Interrupt;
    QWidget *centralWidget;
    QToolBar *toolBar;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *FileSearch;
    QTreeView *FileView;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_2;
    QListWidget *DebugLog;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents_2;
    QHBoxLayout *horizontalLayout_3;
    QWidget *SceneWidget;
    QDockWidget *SceneTree;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_3;
    QTreeView *SceneTreeView;
    QDockWidget *dockWidget_4;
    QWidget *dockWidgetContents_5;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *ComTab;
    QWidget *tab;
    QWidget *tab_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1077, 645);
        MainWindow->setMinimumSize(QSize(600, 400));
        MainWindow->setBaseSize(QSize(0, 0));
        QFont font;
        font.setFamily(QString::fromUtf8("Adobe \345\256\213\344\275\223 Std L"));
        font.setPointSize(12);
        MainWindow->setFont(font);
        MainWindow->setMouseTracking(false);
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8("C:/Users/\346\235\250\345\221\250/Desktop/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setWindowOpacity(1);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QStringLiteral(""));
        MainWindow->setIconSize(QSize(30, 30));
        MainWindow->setToolButtonStyle(Qt::ToolButtonIconOnly);
        MainWindow->setAnimated(false);
        MainWindow->setDocumentMode(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setDockNestingEnabled(true);
        MainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks);
        Run = new QAction(MainWindow);
        Run->setObjectName(QStringLiteral("Run"));
        Run->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/qss_icons/rc/55_0005s_0001_play.png"), QSize(), QIcon::Normal, QIcon::Off);
        Run->setIcon(icon1);
        Run->setAutoRepeat(false);
        Next = new QAction(MainWindow);
        Next->setObjectName(QStringLiteral("Next"));
        Next->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/qss_icons/rc/55_0005s_0003_next.png"), QSize(), QIcon::Normal, QIcon::Off);
        Next->setIcon(icon2);
        Next->setAutoRepeat(false);
        Interrupt = new QAction(MainWindow);
        Interrupt->setObjectName(QStringLiteral("Interrupt"));
        Interrupt->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/qss_icons/rc/55_0005s_0002_pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        Interrupt->setIcon(icon3);
        Interrupt->setAutoRepeat(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        MainWindow->setCentralWidget(centralWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1077, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu->setGeometry(QRect(346, 115, 137, 54));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(menu->sizePolicy().hasHeightForWidth());
        menu->setSizePolicy(sizePolicy);
        menu->setTearOffEnabled(false);
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_2->setGeometry(QRect(316, 153, 137, 54));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_3->setGeometry(QRect(362, 153, 149, 132));
        MainWindow->setMenuBar(menuBar);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QStringLiteral("dockWidget_2"));
        dockWidget_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        dockWidget_2->setAcceptDrops(false);
        dockWidget_2->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        horizontalLayout_2 = new QHBoxLayout(dockWidgetContents);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(7);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setTextFormat(Qt::PlainText);
        label->setScaledContents(false);

        horizontalLayout->addWidget(label);

        FileSearch = new QLineEdit(dockWidgetContents);
        FileSearch->setObjectName(QStringLiteral("FileSearch"));
        FileSearch->setClearButtonEnabled(true);

        horizontalLayout->addWidget(FileSearch);


        verticalLayout->addLayout(horizontalLayout);

        FileView = new QTreeView(dockWidgetContents);
        FileView->setObjectName(QStringLiteral("FileView"));
        FileView->setAcceptDrops(true);
        FileView->setAutoFillBackground(false);
        FileView->setFrameShape(QFrame::NoFrame);
        FileView->setDragEnabled(true);
        FileView->setDragDropMode(QAbstractItemView::DragDrop);
        FileView->setDefaultDropAction(Qt::MoveAction);
        FileView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        FileView->setAutoExpandDelay(1000);
        FileView->setRootIsDecorated(true);
        FileView->setUniformRowHeights(true);
        FileView->setSortingEnabled(true);
        FileView->setAnimated(false);
        FileView->setAllColumnsShowFocus(true);
        FileView->header()->setVisible(false);
        FileView->header()->setCascadingSectionResizes(false);

        verticalLayout->addWidget(FileView);


        horizontalLayout_2->addLayout(verticalLayout);

        dockWidget_2->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_2);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        DebugLog = new QListWidget(dockWidgetContents_4);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/qss_icons/rc/close-pressed.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(DebugLog);
        __qlistwidgetitem->setIcon(icon4);
        DebugLog->setObjectName(QStringLiteral("DebugLog"));
        DebugLog->setProperty("showDropIndicator", QVariant(false));
        DebugLog->setViewMode(QListView::ListMode);
        DebugLog->setSelectionRectVisible(true);

        verticalLayout_2->addWidget(DebugLog);

        dockWidget->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget);
        dockWidget_3 = new QDockWidget(MainWindow);
        dockWidget_3->setObjectName(QStringLiteral("dockWidget_3"));
        dockWidget_3->setMouseTracking(true);
        dockWidget_3->setTabletTracking(true);
        dockWidget_3->setFocusPolicy(Qt::ClickFocus);
        dockWidget_3->setFloating(false);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        horizontalLayout_3 = new QHBoxLayout(dockWidgetContents_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        SceneWidget = new QWidget(dockWidgetContents_2);
        SceneWidget->setObjectName(QStringLiteral("SceneWidget"));

        horizontalLayout_3->addWidget(SceneWidget);

        dockWidget_3->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_3);
        SceneTree = new QDockWidget(MainWindow);
        SceneTree->setObjectName(QStringLiteral("SceneTree"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        SceneTreeView = new QTreeView(dockWidgetContents_3);
        SceneTreeView->setObjectName(QStringLiteral("SceneTreeView"));
        SceneTreeView->setUniformRowHeights(true);
        SceneTreeView->header()->setVisible(false);

        verticalLayout_3->addWidget(SceneTreeView);

        SceneTree->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), SceneTree);
        dockWidget_4 = new QDockWidget(MainWindow);
        dockWidget_4->setObjectName(QStringLiteral("dockWidget_4"));
        sizePolicy.setHeightForWidth(dockWidget_4->sizePolicy().hasHeightForWidth());
        dockWidget_4->setSizePolicy(sizePolicy);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        sizePolicy.setHeightForWidth(dockWidgetContents_5->sizePolicy().hasHeightForWidth());
        dockWidgetContents_5->setSizePolicy(sizePolicy);
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        ComTab = new QTabWidget(dockWidgetContents_5);
        ComTab->setObjectName(QStringLiteral("ComTab"));
        ComTab->setTabPosition(QTabWidget::East);
        ComTab->setTabShape(QTabWidget::Rounded);
        ComTab->setElideMode(Qt::ElideNone);
        ComTab->setUsesScrollButtons(false);
        ComTab->setDocumentMode(false);
        ComTab->setTabsClosable(true);
        ComTab->setMovable(true);
        ComTab->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        ComTab->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        ComTab->addTab(tab_2, QString());

        verticalLayout_4->addWidget(ComTab);

        dockWidget_4->setWidget(dockWidgetContents_5);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_4);

        toolBar->addAction(Interrupt);
        toolBar->addAction(Run);
        toolBar->addAction(Next);
        toolBar->addSeparator();
        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu_3->addAction(Run);
        menu_3->addAction(Next);
        menu_3->addAction(Interrupt);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "WellPlay Editor", Q_NULLPTR));
        Run->setText(QApplication::translate("MainWindow", "\350\277\220\350\241\214", Q_NULLPTR));
        Next->setText(QApplication::translate("MainWindow", "\344\270\213\344\270\200\345\270\247", Q_NULLPTR));
        Interrupt->setText(QApplication::translate("MainWindow", "\346\232\202\345\201\234", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\350\277\220\350\241\214", Q_NULLPTR));
        dockWidget_2->setWindowTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266\347\256\241\347\220\206\345\231\250", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\346\220\234\347\264\242\346\226\207\344\273\266\357\274\232", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "\350\260\203\350\257\225\350\276\223\345\207\272", Q_NULLPTR));

        const bool __sortingEnabled = DebugLog->isSortingEnabled();
        DebugLog->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = DebugLog->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "\351\235\231\346\200\201\350\276\223\345\207\272", Q_NULLPTR));
        DebugLog->setSortingEnabled(__sortingEnabled);

        dockWidget_3->setWindowTitle(QApplication::translate("MainWindow", "\345\234\272\346\231\257\350\247\206\345\233\276", Q_NULLPTR));
        SceneTree->setWindowTitle(QApplication::translate("MainWindow", "\345\234\272\346\231\257\346\240\221", Q_NULLPTR));
        dockWidget_4->setWindowTitle(QApplication::translate("MainWindow", "\347\273\204\344\273\266\350\247\206\345\233\276", Q_NULLPTR));
        ComTab->setTabText(ComTab->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", Q_NULLPTR));
        ComTab->setTabText(ComTab->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
