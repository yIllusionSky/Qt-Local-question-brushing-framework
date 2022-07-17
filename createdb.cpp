#include "createdb.h"
#include "ui_createdb.h"
#include"database.h"
#include<QFileDialog>
#include<QMessageBox>
createDB::createDB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createDB)
{
    ui->setupUi(this);
    this->setFixedSize(450,630);
}

createDB::~createDB()
{
    delete ui;
}

void createDB::on_pushButton_3_pressed()
{
    auto list=QFileDialog::getOpenFileNames(this,tr("请选择文件"),nullptr,tr("文本文件(*txt);;"
                                                                        "EXCEL表格(*xls *xlsx);;"
                                                                        "数据库(*db)"
                                                                        )).toList();
    if(list.empty()) return;
    QString split1=ui->splitz->text().isEmpty()?"\\n":ui->splitz->text();
    QString split2=ui->splitz_2->text().isEmpty()?"\t":ui->splitz_2->text();
    bool showInfo;
    if(ui->checkBox_6->isChecked())
    {
        auto data=DataBase::get();
        showInfo=data->loadDataFromFile(list,split1,split2);
    }
    else
    {


        QList<QString> qstr;
        qstr.clear();
        if(ui->wd1->text().isEmpty())
        {
            qstr.append("default");
        }
        else qstr.append(ui->wd1->text());
        if(ui->wd2->text().isEmpty())
            qstr.append("default");
        else
            qstr.append(ui->wd2->text());
        if(ui->wd3->text().isEmpty())qstr.append("127.0.0.1");
        else
            qstr.append(ui->wd3->text());
        if(ui->wd4->text().isEmpty()) qstr.append("9397");
        else qstr.append(ui->wd4->text());
        if(!ui->wd5->toPlainText().isEmpty())qstr.append(ui->wd5->toPlainText());



        showInfo=DataBase::createNewDataFromFile(ui->wd6->text(),list,qstr,split1,split2);
        DataBase::get();
    }
    if(showInfo==false)
    {
        QMessageBox::warning(this,"信息","创建错误");
    }else
    {
        QMessageBox::information(this,"信息","创建成功");
    }


}


void createDB::on_pushButton_2_pressed()
{
    close();
}

void createDB::closeEvent(QCloseEvent *event)
{
    emit parentDo();
}

