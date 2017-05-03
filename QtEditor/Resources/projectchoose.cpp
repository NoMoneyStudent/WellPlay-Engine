#include "projectchoose.h"
#include "ui_projectchoose.h"
#include <QFileDialog>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>

ProjectChoose::ProjectChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectChoose)
{
    ui->setupUi(this);
    QFile file("projectPath.xml");
    QDomDocument doc;

    if(!file.exists())
    {
        file.open(QFile::WriteOnly|QFile::Text);
        QDomProcessingInstruction ins=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(ins);
        QDomElement root=doc.createElement("ProjectPath");
        doc.appendChild(root);
        QTextStream out(&file);
        doc.save(out,2);
    }
    else
    {
        file.open(QFile::ReadOnly|QFile::Text);
        doc.setContent(&file);

        QDomElement root=doc.documentElement();
        QDomNode node=root.firstChild();

        while(!node.isNull())
        {
            if(node.isElement())
            {
                QDomElement element=node.toElement();
                ui->PathList->addItem(element.text());
                node=node.nextSibling();
            }
        }
    }
    file.close();
}

ProjectChoose::~ProjectChoose()
{
    delete ui;
}

void ProjectChoose::on_pushButton_clicked()
{
    QString path=QFileDialog::getExistingDirectory(this,"选择项目路径");
    if(!path.isNull())
    {
        ui->PathLabel->setText(path);
        QFile file("projectPath.xml");
        file.open(QFile::ReadWrite|QFile::Text);
        QDomDocument doc;
        doc.setContent(&file);
        file.seek(0);
        QDomElement root=doc.documentElement();

        QDomElement pathnode=doc.createElement("path");
        pathnode.appendChild(doc.createTextNode(path));
        root.appendChild(pathnode);

        QTextStream out(&file);
        doc.save(out,2);
        file.close();
    }
}
