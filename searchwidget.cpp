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

#include "searchwidget.h"
#include"ui_search.h"
#include"source.hpp"
#include"mytextedit.h"
#include<QKeyEvent>
#include"database.h"
#include<QStandardItem>
#include"showinformation.h"
searchWidget::searchWidget(QWidget *parent) : QWidget(parent),ui(new Ui::searchWidget)
{
    ui->setupUi(this);
    this->setFixedSize(450,630);
    model=new QStandardItemModel;

    myInformation=new ShowInformation(this);
    connect(myInformation,&ShowInformation::parentDo,this,&searchWidget::sandDo);
    myInformation->hide();
}

searchWidget::~searchWidget()
{
    model->clear();
    disconnect(myInformation,&ShowInformation::parentDo,this,&searchWidget::sandDo);
    delete model;
    delete ui;
    delete myInformation;
}
void searchWidget::closeEvent(QCloseEvent *event)
{

    emit parentDo();
}

void searchWidget::on_pushButton_pressed()
{
    QString qtext=ui->searchEdit->toPlainText();
    auto liststr=qtext.split(' ',Qt::SkipEmptyParts);
    auto data=DataBase::get();
    model->clear();
    myProblem.clear();
    myProblemId.clear();
    isRecord.clear();
    QStandardItem* list;
    data->searchData(liststr,myProblem,myProblemId,(SearchWay)(ui->check->checkState()==Qt::Checked));
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
    /*加载数据*/
    ui->topicView->setModel(model);

}


void searchWidget::on_pushButton_2_pressed()
{
    close();
}


void searchWidget::on_topicView_doubleClicked(const QModelIndex &index)
{
    myInformation->setShowInformation(&myProblem,&myProblemId,&isRecord,index.row());
    myInformation->show();
    myInformation->raise();
    myInformation->activateWindow();
}

void searchWidget::sandDo(quint32 theIndex, quint8 whatHappen)
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

