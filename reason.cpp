//    This file is part of ComputerExerciseProblems
//    Copyright (C) 2021  IllusionAutumn

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "reason.h"
#include"ui_reason.h"
#include"showinformation.h"
MyReason::MyReason(QWidget* parent,quint32 times,QList<quint8>* chooseAnswer,QList<ProblemData>* problem,QList<quint32>* problemId):QWidget(parent),ui(new Ui::reason)
{
    score=0;
    ui->setupUi(this);
    this->setFixedSize(450,630);
    model=new QStandardItemModel;
    myChooseAnswer=chooseAnswer;
    myProblem=problem;
    myProblemId=problemId;
    QStandardItem* list;
    isRecord.resize(myProblem->size());
    DataBase* data=DataBase::get();
    data->panDataFromRecord(*problemId,isRecord);
    for(quint32 index=0;index<myProblem->size();index++)
    {
        if((*myProblem)[index].answer==(*myChooseAnswer)[index])
        {
            list=new QStandardItem(QIcon(":/Image/Y.png"),(*myProblem)[index].topic);
            score++;
        }
        else
        {
            list=new QStandardItem(QIcon(":/Image/N.png"),(*myProblem)[index].topic);
        }
        if(isRecord[index])
        {
            list->setForeground(QBrush(QColor(255, 0, 255)));
        }

        model->appendRow(list);
    }
    ui->topicView->setModel(model);

    ui->score->setText(QString::number(score*3)+"分");
    quint32 hour=times/3600;
    quint32 minute=times/60%60;
    quint32 sec=times%60;
    ui->timeS->setText(QString::asprintf("%02d:%02d:%02d",hour,minute,sec));
    ui->countS->setText(QString::asprintf("%2d/%2lld",score,myProblem->size()));
    myInformation=new ShowInformation(this);
    connect(myInformation,&ShowInformation::parentDo,this,&MyReason::sandDo);
    myInformation->hide();

}

MyReason::~MyReason()
{
    model->clear();
    disconnect(myInformation,&ShowInformation::parentDo,this,&MyReason::sandDo);
    delete model;
    delete ui;
    delete myInformation;
}

void MyReason::on_topicView_doubleClicked(const QModelIndex &index)
{
    myInformation->setShowInformation(myProblem,myChooseAnswer,myProblemId,&isRecord,index.row());
    myInformation->show();
    myInformation->raise();
    myInformation->activateWindow();
}

void MyReason::sandDo(quint32 theIndex,quint8 whatHappend)
{
    if(whatHappend==0) return;
    if(whatHappend&32)
    {
        if(whatHappend&1)
        {
            score--;
        }
        model->removeRow(theIndex);
        myProblem->removeAt(theIndex);
        myProblemId->removeAt(theIndex);
        myChooseAnswer->removeAt(theIndex);
        ui->score->setText(QString::number(score*3)+"分");
        ui->countS->setText(QString::asprintf("%2d/%2lld",score,myProblem->size()));
        return;

    }
    if(whatHappend&1)
    {
        score++;
        ui->score->setText(QString::number(score*3)+"分");
        ui->countS->setText(QString::asprintf("%2d/%2lld",score,myProblem->size()));
        auto one=model->itemFromIndex(model->index(theIndex,0));
        one->setIcon(QIcon(":/Image/Y.png"));
    }
    else if(whatHappend&2)
    {
        score--;
        ui->score->setText(QString::number(score*3)+"分");
        ui->countS->setText(QString::asprintf("%2d/%2lld",score,myProblem->size()));
        auto one=model->itemFromIndex(model->index(theIndex,0));
        one->setIcon(QIcon(":/Image/N.png"));
    }
    if(whatHappend&4)
    {
        auto one=model->itemFromIndex(model->index(theIndex,0));
        one->setForeground(QBrush(QColor(0, 0, 0)));
    }
    else if(whatHappend&8)
    {
        auto one=model->itemFromIndex(model->index(theIndex,0));
        one->setForeground(QBrush(QColor(255, 0, 255)));
    }
    if(whatHappend&16)
    {
        auto one=model->itemFromIndex(model->index(theIndex,0));
        one->setText((*myProblem)[theIndex].topic);
    }
}

void MyReason::closeEvent(QCloseEvent *event)
{
    emit closeParent();
}


void MyReason::on_pushButton_pressed()
{
    this->close();
}



void MyReason::on_checkBox_stateChanged(int arg1)
{
     if(arg1==Qt::CheckState::Checked)
     {
         for(quint32 index=0;index<myProblem->size();index++)
         {
             if(ui->topicView->isRowHidden(index))
             {
                 ui->topicView->setRowHidden(index,false);
             }
         }
     }
     else
     {
         for(quint32 index=0;index<myProblem->size();index++)
         {
             if((*myProblem)[index].answer==(*myChooseAnswer)[index])
             {
                ui->topicView->setRowHidden(index,true);
             }
         }
     }
}

