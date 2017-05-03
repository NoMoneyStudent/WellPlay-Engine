#include "mainwindow.h"
#include "projectchoose.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

#pragma comment(lib,"WellPlay.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f(":/qdarkstyle/style.qss");
    if (!f.exists())
    {
        qDebug()<<"Unable to set stylesheet, file not found\n";
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    ProjectChoose p;
	p.exec();

    MainWindow w;
    w.show();
	
    a.exec();
}
