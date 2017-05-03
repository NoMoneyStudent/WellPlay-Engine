#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeView>
#include <QSettings>
#include <QDebug>
#include <QTextCodec>
#include <qpushbutton.h>
#include <qaction.h>
#include <qboxlayout.h>
#include <qstandarditemmodel.h>

#include "WellPlayMain.h"
#include "EngineRuntime\EngineUtility.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    f(new QFileSystemModel)
{
    ui->setupUi(this);

	EngineCallBack::OnLog =
		[&](const std::wstring& data) {emit onlog(data); };
	EngineCallBack::OnFinishUpdate =
		[&]() {emit onEngineFinishUpdate(); };

	qRegisterMetaType<QVector<int>>("QVector<int>");
	qRegisterMetaType<std::shared_ptr<GameObject>>("std::shared_ptr<GameObject>");

	scenetree = new SceneTreeControll(ui->SceneTree);

	ui->ComTab->removeTab(1);
	ui->ComTab->removeTab(0);
	comview = new ComponentViewControll(ui->ComTab);

    QWidget* p = takeCentralWidget();
    if(p) delete p;
	
	engine = std::thread(EngineMain, GetHWND(), GetInput());

    f->setRootPath("E:/");
    f->setReadOnly(false);

    ui->FileView->setModel(f);
    ui->FileView->setRootIndex(f->index("E:/"));
	ui->FileView->header()->hide();
	ui->FileView->hideColumn(1);
	ui->FileView->hideColumn(2);
	ui->FileView->hideColumn(3);

    QSettings setting("MySetting", "MyApp");
    if(!setting.value("geometry").isNull())
        restoreGeometry(setting.value("geometry").toByteArray());
    if(!setting.value("state").isNull())
        restoreState(setting.value("state").toByteArray());

     QList<QDockWidget *> dwList = this->findChildren<QDockWidget*>();
     foreach (QDockWidget *dw, dwList)
     {
		dw->updateGeometry();
     }
	 connect(qApp, &QApplication::focusChanged, this, &MainWindow::SceneFocus);
	 connect(ui->Run, &QAction::triggered, this, &MainWindow::EnginePlay);
	 connect(this, &MainWindow::onlog, this, &MainWindow::Log);
	 connect(this, &MainWindow::onEngineFinishUpdate, this, &MainWindow::UpdateComponents);
	 connect(scenetree, &SceneTreeControll::OnSelect, comview, &ComponentViewControll::OnSelect);
	 connect(scenetree, &SceneTreeControll::OnRemoveComView, comview, &ComponentViewControll::OnRemoveComView);
}

MainWindow::~MainWindow()
{
	ShutDown();

    QSettings setting("MySetting", "MyApp");

	QList<QDockWidget *> dwList = this->findChildren<QDockWidget*>();
	foreach(QDockWidget *dw, dwList)
	{
		dw->updateGeometry();
	}
	setting.setValue("geometry", saveGeometry());
	setting.setValue("state", saveState());

    delete ui;
	delete f;
	delete scenetree;
	delete comview;

	engine.join();
}

HWND MainWindow::GetHWND() const
{
	return reinterpret_cast<HWND>(ui->SceneWidget->winId()); 
}

HWND MainWindow::GetInput() const
{
	return reinterpret_cast<HWND>(winId());
}

void MainWindow::UpdateComponents()
{
	auto lk = Interrupt();
	comview->ReadData();
	lk.unlock();
	comview->UpdateData();
}

void MainWindow::SceneFocus(QWidget* old,QWidget* now)
{
	if (old == ui->dockWidget_3)
	{
		SetFocus(false);
	}
	else if (now == ui->dockWidget_3)
	{
		SetFocus(true);
	}
}

void MainWindow::EnginePlay(bool ifplay)
{
	auto lk = Interrupt();
	EngineUtility::SetPlay(ifplay);
}

void MainWindow::on_FileSearch_textChanged(const QString &arg1)
{
    QStringList strlist;
    strlist<<arg1;
    f->setNameFilters(strlist);
    f->setNameFilterDisables(false);
}

void MainWindow::Log(const std::wstring& data)
{
    QIcon icon4;
    icon4.addFile(QStringLiteral(":/qss_icons/rc/close-pressed.png"), QSize(), QIcon::Normal, QIcon::Off);
    QListWidgetItem *qlistwidgetitem = new QListWidgetItem(ui->DebugLog);
    qlistwidgetitem->setIcon(icon4);
    qlistwidgetitem->setText(QString::fromStdWString(data));
}
