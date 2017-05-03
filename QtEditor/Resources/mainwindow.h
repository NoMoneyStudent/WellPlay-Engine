#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include "SceneTreeControll.h"
#include "ComponentViewControll.h"
#include <thread>
#include <memory>

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
	void onlog(const std::wstring data);
	void onEngineFinishUpdate();

public slots:
	void Log(const std::wstring& data);
	void UpdateComponents();

private slots:
    void on_FileSearch_textChanged(const QString &arg1);
	void SceneFocus(QWidget* old, QWidget* now);
	void EnginePlay(bool ifplay);

private:
    Ui::MainWindow* ui;
    QFileSystemModel* f;
	SceneTreeControll* scenetree;
	ComponentViewControll* comview;

	std::thread engine;
};

#endif // MAINWINDOW_H
