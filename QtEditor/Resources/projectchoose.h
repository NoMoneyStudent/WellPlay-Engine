#ifndef PROJECTCHOOSE_H
#define PROJECTCHOOSE_H

#include <QDialog>

namespace Ui {
class ProjectChoose;
}

class ProjectChoose : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectChoose(QWidget *parent = 0);
    ~ProjectChoose();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProjectChoose *ui;
};

#endif // PROJECTCHOOSE_H
