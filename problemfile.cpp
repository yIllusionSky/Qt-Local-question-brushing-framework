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

#include"ui_problem.h"
#include<problemfile.h>
#include<QKeyEvent>
#include<reason.h>
#include<QTimer>
problemFile::problemFile(QWidget *parent,TopicEnum enumtype) : QWidget(parent),ui(new Ui::TestWindow)
{

    second=0;
    choose=enumtype;
    DataBase* data=DataBase::get();
    answerCount=data->selectIndexFromUInt(InfoType::OneCount);
    if((quint8)enumtype&(quint8)TopicEnum::Continue)
    {
         data->getData(this->problem,this->problemId,data->selectIndexFromUInt(InfoType::Index),answerCount);
    }
    else if((quint8)enumtype&(quint8)TopicEnum::ErrorTopic)
    {
        data->getDataFromError(problem,problemId,answerCount);
    }
    else if((quint8)enumtype&(quint8)TopicEnum::RecordTopic)
    {
        data->getDataFromRecord(problem,problemId,answerCount);
    }
    else if((quint8)enumtype&(quint8)TopicEnum::Topic)
    {
        data->getData(problem,problemId,answerCount);
    }
    else
    {
        close();
        return;
    }
    ui->setupUi(this);
    this->setFixedSize(450,630);
    this->setFocus();
    autoNext=false;
    connect(ui->A,&QPushButton::pressed,this,&problemFile::chooseID);
    connect(ui->B,&QPushButton::pressed,this,&problemFile::chooseID);
    connect(ui->C,&QPushButton::pressed,this,&problemFile::chooseID);
    connect(ui->D,&QPushButton::pressed,this,&problemFile::chooseID);
    timer=new QTimer(this);
    connect(timer, &QTimer::timeout, this, &problemFile::updateTimer);
    timer->start(1000);
    chooseAnswer.resize(problem.size());
    for(quint32 is=0;is<problem.size();is++)
    {
        chooseAnswer[is]=problem[is].answer&16;
    }
    setIndex(0);

    ui->label_3->setText(QString::asprintf("%2d/%2d",index,answerCount));

}

problemFile::~problemFile()
{
    if(!this->problem.isEmpty())
    {
        if(timer->isActive())timer->stop();

        disconnect(ui->A,&QPushButton::pressed,this,&problemFile::chooseID);
        disconnect(ui->B,&QPushButton::pressed,this,&problemFile::chooseID);
        disconnect(ui->C,&QPushButton::pressed,this,&problemFile::chooseID);
        disconnect(ui->D,&QPushButton::pressed,this,&problemFile::chooseID);
        disconnect(timer, &QTimer::timeout, this, &problemFile::updateTimer);

        delete timer;
        chooseAnswer.clear();
        problem.clear();
        problemId.clear();
    }


    delete ui;
}

void problemFile::on_pushButton_2_pressed()
{
    close();

}

void problemFile::chooseID()
{

    auto push=qobject_cast<QPushButton*>(sender());
    //Find标志了哪个被选择
    quint8 find=push->whatsThis().toUInt();
    if(chooseAnswer[index]&16)
    {
        if((chooseAnswer[index]&find)==0)
        {
            push->setStyleSheet("border:none;\nbackground-color: rgb(255, 170, 255);");
            chooseAnswer[index]|=find;
        }
        else
        {
            chooseAnswer[index]&=~find;
            push->setStyleSheet("border:none;\nbackground-color: rgb(180, 230, 255);");
        }
    }
    else
    {
        if(chooseAnswer[index]!=0)
        {
            if(chooseAnswer[index]!=find)
            {
                push->setStyleSheet("border:none;\nbackground-color: rgb(255, 170, 255);");

                for(auto child:ui->horizontalFrame->findChildren<QPushButton*>())
                {
                    if(child->whatsThis().toUInt()==chooseAnswer[index])
                    {
                        child->setStyleSheet("border:none;\nbackground-color: rgb(180, 230, 255);");
                    }
                }
                chooseAnswer[index]=find;
            }
        }
        else
        {
            chooseAnswer[index]=find;
            push->setStyleSheet("border:none;\nbackground-color: rgb(255, 170, 255);");
            if(autoNext)
            {
                on_pushButton_4_pressed();
            }
        }
    }

}

void problemFile::setIndex(quint32 theIndex)
{

    index=theIndex;
    //加载选择的样式

    //载入题目
    ui->Topic->setText(problem[index].topic);

    ui->AnswerA->setText(problem[index].option[0]);
    ui->AnswerB->setText(problem[index].option[1]);
    if(problem[index].option.size()<3)
    {
        ui->AnswerC->setText("");
        ui->AnswerC->setEnabled(false);
        ui->C->setEnabled(false);
    }else
    {
        ui->AnswerC->setEnabled(true);
        ui->AnswerC->setText(problem[index].option[2]);
        ui->C->setEnabled(true);
    }
    if(problem[index].option.size()<4)
    {
        ui->AnswerD->setText("");
        ui->AnswerD->setEnabled(false);
        ui->D->setEnabled(false);
    }
    else
    {
        ui->AnswerD->setEnabled(true);
        ui->AnswerD->setText(problem[index].option[3]);
        ui->D->setEnabled(true);
    }
    if((chooseAnswer[index]&1)==0)
    {
        ui->A->setStyleSheet("border:none;\nbackground-color: rgb(180, 230, 255);");
    }
    else
    {
        ui->A->setStyleSheet("border:none;\nbackground-color: rgb(255, 170, 255);");
    }
    if((chooseAnswer[index]&2)==0)
    {
        ui->B->setStyleSheet("border:none;\nbackground-color: rgb(180, 230, 255);");
    }
    else
    {
        ui->B->setStyleSheet("border:none;\nbackground-color: rgb(255, 170, 255);");
    }
    if((chooseAnswer[index]&4)==0)
    {

        ui->C->setStyleSheet("border:none;\nbackground-color: rgb(180, 230, 255);");
    }
    else
    {
        ui->C->setStyleSheet("border:none;\nbackground-color: rgb(255, 170, 255);");
    }
    if((chooseAnswer[index]&8)==0)
    {
        ui->D->setStyleSheet("border:none;\nbackground-color: rgb(180, 230, 255);");
    }
    else
    {
        ui->D->setStyleSheet("border:none;\nbackground-color: rgb(255, 170, 255);");
    }

}

void problemFile::keyPressEvent(QKeyEvent *keyEvent)
{

    if (keyEvent->key() ==Qt::Key_1)
    {
       ui->A->pressed();
    }
    else if (keyEvent->key()==Qt::Key_2)
    {
        ui->B->pressed();
    }
    else if (keyEvent->key()==Qt::Key_3)
    {
        ui->C->pressed();
    }
    else if (keyEvent->key()==Qt::Key_4)
    {
        ui->D->pressed();
    }
    else if(keyEvent->key()==Qt::Key_Enter)
    {
        on_pushButton_4_pressed();
    }
}

void problemFile::closeEvent(QCloseEvent *event)
{
    emit parentDo();

}


void problemFile::on_checkBox_stateChanged(int arg1)
{
    autoNext=Qt::CheckState::Checked==arg1;
}


void problemFile::on_pushButton_3_pressed()
{
    if(index>0) {
        ui->label_3->setText(QString::asprintf("%2d/%2d",index,answerCount));
        setIndex(index-1);
    }
}


void problemFile::on_pushButton_4_pressed()
{
    if(chooseAnswer[index]==0)
    {
        return;
    }
    if(index==answerCount-1)
    {
        timer->stop();
        QList<bool> ok;
        DataBase* data=DataBase::get();
        for(quint32 sz=0;sz<problem.size();sz++)
        {
            ok.append(problem[sz].answer==chooseAnswer[sz]);
        }
        data->updateDataFromError(problemId,ok);
        if((quint8)choose&(quint8)TopicEnum::Continue)
        {
            data->updateIndexFromUInt(data->selectIndexFromUInt()+data->selectIndexFromUInt(InfoType::OneCount));
        }
        data->updateIndexFromUInt(data->selectIndexFromUInt(InfoType::Count)+data->selectIndexFromUInt(InfoType::OneCount));
        MyReason* rea=new MyReason(this,second,&chooseAnswer,&problem,&problemId);
        connect(rea,&MyReason::closeParent,this,&problemFile::freeType);
        rea->show();
        rea->raise();
        rea->activateWindow();
        return;
    }
    setIndex(index+1);
    ui->label_3->setText(QString::asprintf("%2d/%2d",index+1,answerCount));
}

void problemFile::updateTimer()
{
    second+=1;
    quint32 hour=second/3600;
    quint32 minute=second/60%60;
    quint32 sec=second%60;
    ui->label_4->setText(QString::asprintf("%02d:%02d:%02d",hour,minute,sec));
}

void problemFile::freeType()
{
    delete sender();
    this->close();

}

