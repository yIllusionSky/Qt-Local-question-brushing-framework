#include "form.h"
#include"ui_form.h"
#include<QRadioButton>
#include<mainwindow.h>
form::form(QWidget *parent) : QWidget(parent),ui(new Ui::Form)
{

    ui->setupUi(this);
    connect(this,&form::ChangeType,qobject_cast<MainWindow*>(this->parent()),&MainWindow::ChangeType);
    for(auto i:ui->groupBox->children())
    {
        if(QString::compare(i->metaObject()->className(),"QRadioButton")==0)
        {
            connect(qobject_cast<QRadioButton*>(i),&QRadioButton::pressed,this,&form::eventHappend);
        }
    }
    chooseType=0;
    setAttribute(Qt::WA_DeleteOnClose);
}

form::~form()
{

    for(auto i:ui->groupBox->children())
    {
        if(QString::compare(i->metaObject()->className(),"QRadioButton")==0)
        {
            disconnect(qobject_cast<QRadioButton*>(i),&QRadioButton::pressed,this,&form::eventHappend);
        }
    }
    disconnect(this,&form::ChangeType,qobject_cast<MainWindow*>(this->parent()),&MainWindow::ChangeType);
    delete ui;
}


void form::eventHappend()
{
    QRadioButton* rad=qobject_cast<QRadioButton*>(sender());
    this->chooseType=rad->objectName().right(1).toInt();
    emit ChangeType(this->chooseType-1);
}
void form::on_pushButton_pressed()
{
    this->close();
}

