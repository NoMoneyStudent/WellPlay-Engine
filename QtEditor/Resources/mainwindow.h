#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include "SceneTreeControll.h"
#include "ComponentViewControll.h"
#include "LogViewControll.h"
#include <thread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	HWND GetHWND() const;
	HWND GetInput() const;
	
signals:
	void Log(const QString data, int mode);
	void onEngineFinishUpdate();

public slots:
	void UpdateComponents();

private slots:
    void on_FileSearch_textChanged(const QString &arg1);
	void SceneFocus(QWidget* old, QWidget* now);
	void EnginePlay(bool ifplay);


    void on_Asetting_triggered();

private:
    Ui::MainWindow* ui;
    QFileSystemModel* f;
	SceneTreeControll* scenetree;
	ComponentViewControll* comview;
	LogViewControll* logview;

	std::thread engine;
};

#endif // MAINWINDOW_H
