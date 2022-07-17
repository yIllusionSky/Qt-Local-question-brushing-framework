#include "editclass.h"
#include "ui_editclass.h"
#include"showinformation.h"
#include<QMessageBox>
EditClass::EditClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditClass)
{
    ui->setupUi(this);
    this->setFixedSize(450,630);
    model=new QStandardItemModel;

    myInformation=new ShowInformation(this);
    connect(myInformation,&ShowInformation::parentDo,this,&EditClass::sandDo);
    myInformation->hide();

    myProblem.clear();
    myProblemId.clear();
    isRecord.clear();
    QStandardItem* list;
    auto data=DataBase::get();
    quint32 dataCount;
    data->getDataCount(&dataCount);
    data->getData(myProblem,myProblemId,dataCount);

    isRecord.resize(myProblemId.size());
    errorRecord.resize(myProblemId.size());
    data->panDataFromRecord(myProblemId,isRecord);
    data->panDataFromError(myProblemId,errorRecord);
    for(quint32 index=0;index<myProblem.size();index++)
    {
        if(myProblemId[index])
        {
            list=new QStandardItem(QIcon(":/Image/Y.png"),myProblem[index].topic);

        }
        else
        {
            list=new QStandardItem(QIcon(":/Image/N.png"),myProblem[index].topic);
        }
        if(isRecord[index])
        {
            list->setForeground(QBrush(QColor(255, 0, 255)));
        }
        model->appendRow(list);
    }
    list=new QStandardItem("添加一行");
    list->setForeground(QBrush(QColor(0,0,255)));
    model->appendRow(list);
    /*加载数据*/
    ui->topicView->setModel(model);
}

EditClass::~EditClass()
{
    model->clear();
    disconnect(myInformation,&ShowInformation::parentDo,this,&EditClass::sandDo);
    delete model;

    delete myInformation;
    delete ui;
}

void EditClass::on_topicView_doubleClicked(const QModelIndex &index)
{

    if(index.row()==myProblem.size())
    {
        ProblemData problem({"缺省",{"缺省","缺省","缺省","缺省"},0,"缺省"});
        auto data=DataBase::get();
        data->insertData(problem);
        data->getLastData(myProblem,myProblemId);
        isRecord.append(false);
        QStandardItem* list;
        list=new QStandardItem(QIcon(":/Image/Y.png"),myProblem[index.row()].topic);
        model->insertRow(index.row(),list);
    }
    myInformation->setShowInformation(&myProblem,&myProblemId,&isRecord,index.row());
    myInformation->show();
    myInformation->raise();
    myInformation->activateWindow();
}


void EditClass::on_pushButton_2_pressed()
{
    close();
}

void EditClass::sandDo(quint32 theIndex, quint8 whatHappen)
{
    auto one=model->itemFromIndex(model->index(theIndex,0));
    one->setText(myProblem[theIndex].topic);
    if(whatHappen&32)
    {
        model->removeRow(theIndex);
        myProblem.removeAt(theIndex);
        myProblemId.removeAt(theIndex);
        return;
    }
}

void EditClass::closeEvent(QCloseEvent *event)
{
    emit parentDo();
}


void EditClass::on_pushButton_3_pressed()
{
    auto data=DataBase::get();
    data->deleteAllData();
    QMessageBox::information(this,"信息","处理成功，请重启");
    close();
}

