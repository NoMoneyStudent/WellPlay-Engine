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
	scenetree = new SceneTreeControll(ui->SceneTree);

    QWidget* p = takeCentralWidget();
    if(p)
       delete p;
	
	EngineCallBack::OnFinishUpdate =
		[&]() {emit onEngineFinishUpdate(); };
	
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
	
	auto go = currentSelect.lock();
	std::string m_name = go->GetName();
	auto transfrom = go->GetTransform().lock();
	auto p = transfrom->GetLocalPosition();
	auto r = transfrom->GetLocalEulerAngles();
	auto s = transfrom->GetLocalScale();
	currentSelect = go;
	lk.unlock();
	ui->GameObjectNameEdit->setText(QString::fromStdString(m_name));
	ui->px->setText(QString::number(p.m128_f32[0]));
	ui->px->setCursorPosition(0);
	ui->py->setText(QString::number(p.m128_f32[1]));
	ui->py->setCursorPosition(0);
	ui->pz->setText(QString::number(p.m128_f32[2]));
	ui->pz->setCursorPosition(0);
	ui->rx->setText(QString::number(r.x));
	ui->rx->setCursorPosition(0);
	ui->ry->setText(QString::number(r.y));
	ui->ry->setCursorPosition(0);
	ui->rz->setText(QString::number(r.z));
	ui->rz->setCursorPosition(0);
	ui->sx->setText(QString::number(s.m128_f32[0]));
	ui->sx->setCursorPosition(0);
	ui->sy->setText(QString::number(s.m128_f32[1]));
	ui->sy->setCursorPosition(0);
	ui->sz->setText(QString::number(s.m128_f32[2]));
	ui->sz->setCursorPosition(0);
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

void MainWindow::SelectGameObject(const QModelIndex & select)
{
	QModelIndex index = select;
	std::vector<int> indexarray;
	while (index.isValid())
	{
		indexarray.push_back(index.row());
		index = index.parent();
	}
	std::reverse(indexarray.begin(), indexarray.end());
	assert(indexarray.size() > 0);

	auto lk = Interrupt();
	auto rootobject = Scene::GetCurrentScene()->GetRootGameObject();
	auto trans = rootobject[indexarray[0]]->GetTransform().lock();
	for (int i = 1; i < indexarray.size(); i++)
	{
		trans = trans->GetChildren()[indexarray[i]];
	}
	auto go = trans->gameobject();
	std::string m_name = go->GetName();
	auto transfrom = go->GetTransform().lock();
	auto p = transfrom->GetLocalPosition();
	auto r = transfrom->GetLocalEulerAngles();
	auto s = transfrom->GetLocalScale();
	currentSelect = go;
	lk.unlock();
	ui->GameObjectNameEdit->setText(QString::fromStdString(m_name));
	ui->px->setText(QString::number(p.m128_f32[0]));
	ui->px->setCursorPosition(0);
	ui->py->setText(QString::number(p.m128_f32[1]));
	ui->py->setCursorPosition(0);
	ui->pz->setText(QString::number(p.m128_f32[2]));
	ui->pz->setCursorPosition(0);
	ui->rx->setText(QString::number(r.x));
	ui->rx->setCursorPosition(0);
	ui->ry->setText(QString::number(r.y));
	ui->ry->setCursorPosition(0);
	ui->rz->setText(QString::number(r.z));
	ui->rz->setCursorPosition(0);
	ui->sx->setText(QString::number(s.m128_f32[0]));
	ui->sx->setCursorPosition(0);
	ui->sy->setText(QString::number(s.m128_f32[1]));
	ui->sy->setCursorPosition(0);
	ui->sz->setText(QString::number(s.m128_f32[2]));
	ui->sz->setCursorPosition(0);

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
