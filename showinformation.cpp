#include "showinformation.h"
#include"ui_showInformation.h"
#include<QMessageBox>
ShowInformation::ShowInformation(QWidget *parent)
    : QWidget(parent),ui(new Ui::showInformation)
{
    ui->setupUi(this);
    this->setFixedSize(450,630);
    connect(ui->A,&QPushButton::pressed,this,&ShowInformation::chooseId);
    connect(ui->B,&QPushButton::pressed,this,&ShowInformation::chooseId);
    connect(ui->C,&QPushButton::pressed,this,&ShowInformation::chooseId);
    connect(ui->D,&QPushButton::pressed,this,&ShowInformation::chooseId);
}

void ShowInformation::setShowInformation(QList<ProblemData>* problem,QList<quint8>* ans,QList<quint32>* id,QList<bool>* theRecord,quint32 theIndex)
{
    tensorType=true;
    ui->A->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    ui->B->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    ui->C->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    ui->D->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    tproblem=problem;
    tans=ans;
    tid=id;
    tRecord=theRecord;
    tIndex=theIndex;
    canChange=false;
    changeType=0;
    justAnswer=(*problem)[theIndex].answer;
    ui->Topic->setText((*problem)[theIndex].topic);
    ui->AnswerA->setText((*problem)[theIndex].option[0]);
    ui->AnswerB->setText((*problem)[theIndex].option[1]);
    if((*problem)[theIndex].option.size()>2)
    {
        ui->AnswerC->setText((*problem)[theIndex].option[2]);
        if((*problem)[theIndex].answer&4)
        {
            ui->C->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else if((*ans)[theIndex]&4)
        {
            ui->C->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
        }
    }
    else
    {
        ui->AnswerC->setEnabled(false);
        ui->C->setEnabled(false);
    }
    if((*problem)[theIndex].option.size()>3)
    {
        ui->AnswerD->setText((*problem)[theIndex].option[3]);
        if((*problem)[theIndex].answer&8)
        {
            ui->D->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else if((*ans)[theIndex]&8)
        {
            ui->D->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
        }
    }
    else
    {
        ui->AnswerD->setEnabled(false);
        ui->D->setEnabled(false);
    }
    ui->why->setText((*problem)[theIndex].reason);
    if((*problem)[theIndex].answer&1)
    {
        ui->A->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
    }
    else if((*ans)[theIndex]&1)
    {
        ui->A->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
    }
    if((*problem)[theIndex].answer&2)
    {
        ui->B->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
    }else if((*ans)[theIndex]&2)
    {
        ui->B->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
    }
    if((*theRecord)[theIndex]==true)
    {
        ui->pushButton_5->setText("取消收藏");
    }
    else
    {
        ui->pushButton_5->setText("收藏");
    }
    ui->pushButton_3->setText("编辑");
    ui->pushButton_4->setText("返回");
    ui->Topic->setReadOnly(true);
    ui->AnswerA->setReadOnly(true);
    ui->AnswerB->setReadOnly(true);
    if(ui->AnswerC->isEnabled())
    {
        ui->AnswerC->setReadOnly(true);
    }
    if(ui->AnswerD->isEnabled())
    {
        ui->AnswerD->setReadOnly(true);
    }
    ui->why->setReadOnly(true);
}

void ShowInformation::setShowInformation(QList<ProblemData> *problem, QList<quint32> *id, QList<bool> *theRecord, quint32 theIndex)
{
    tensorType=false;
    ui->A->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    ui->B->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    ui->C->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    ui->D->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
    tproblem=problem;
    tid=id;
    tRecord=theRecord;
    tIndex=theIndex;
    canChange=false;
    changeType=0;
    justAnswer=(*problem)[theIndex].answer;
    ui->Topic->setText((*problem)[theIndex].topic);
    ui->AnswerA->setText((*problem)[theIndex].option[0]);
    ui->AnswerB->setText((*problem)[theIndex].option[1]);
    if((*problem)[theIndex].option.size()>2)
    {
        ui->AnswerC->setText((*problem)[theIndex].option[2]);
        if((*problem)[theIndex].answer&4)
        {
            ui->C->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
    }
    else
    {
        ui->AnswerC->setEnabled(false);
        ui->C->setEnabled(false);
    }
    if((*problem)[theIndex].option.size()>3)
    {
        ui->AnswerD->setText((*problem)[theIndex].option[3]);
        if((*problem)[theIndex].answer&8)
        {
            ui->D->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
    }
    else
    {
        ui->AnswerD->setEnabled(false);
        ui->D->setEnabled(false);
    }
    ui->why->setText((*problem)[theIndex].reason);
    if((*problem)[theIndex].answer&1)
    {
        ui->A->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
    }

    if((*problem)[theIndex].answer&2)
    {
        ui->B->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");

        if((*theRecord)[theIndex]==true)
        {
            ui->pushButton_5->setText("取消收藏");
        }
        else
        {
            ui->pushButton_5->setText("收藏");
        }
        ui->pushButton_3->setText("编辑");
        ui->pushButton_4->setText("返回");
        ui->Topic->setReadOnly(true);
        ui->AnswerA->setReadOnly(true);
        ui->AnswerB->setReadOnly(true);
        if(ui->AnswerC->isEnabled())
        {
            ui->AnswerC->setReadOnly(true);
        }
        if(ui->AnswerD->isEnabled())
        {
            ui->AnswerD->setReadOnly(true);
        }
        ui->why->setReadOnly(true);
    }
}


ShowInformation::~ShowInformation()
{
    disconnect(ui->A,&QPushButton::pressed,this,&ShowInformation::chooseId);
    disconnect(ui->B,&QPushButton::pressed,this,&ShowInformation::chooseId);
    disconnect(ui->C,&QPushButton::pressed,this,&ShowInformation::chooseId);
    disconnect(ui->D,&QPushButton::pressed,this,&ShowInformation::chooseId);
    delete ui;
}

void ShowInformation::on_pushButton_4_pressed()
{
    if(canChange==true)
    {
        if(QMessageBox::warning(this,"提示","你确定要删除？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        {
            changeType=32;
            if(tensorType&&(*tproblem)[tIndex].answer==(*tans)[tIndex])
            {
                changeType++;
            }
            auto data=DataBase::get();
            data->deleteData((*tid)[tIndex]);
            close();
        }
    }
    else close();
}


void ShowInformation::on_pushButton_5_pressed()
{
    if((*tRecord)[tIndex])
    {
        (*tRecord)[tIndex]=false;
        changeType|=4;
        changeType&=~8;
        ui->pushButton_5->setText("收藏");
    }
    else
    {
        (*tRecord)[tIndex]=true;
        changeType|=8;
        changeType&=~4;
        ui->pushButton_5->setText("取消收藏");
    }
    DataBase* data=DataBase::get();
    data->updateDataFromRecord((*tid)[tIndex]);
}


void ShowInformation::on_pushButton_3_pressed()
{
    if(canChange==false)
    {
        canChange=true;
        if((*tproblem)[tIndex].answer&1)
        {
            ui->A->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->A->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
        }
        if((*tproblem)[tIndex].answer&2)
        {
            ui->B->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->B->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
        }
        if((*tproblem)[tIndex].option.size()>2&&(*tproblem)[tIndex].answer&4)
        {
            ui->C->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->C->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
        }
        if((*tproblem)[tIndex].option.size()>3&&(*tproblem)[tIndex].answer&8)
        {
            ui->D->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->D->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
        }
        ui->pushButton_3->setText("确认");
        ui->pushButton_4->setText("删除");
        ui->Topic->setReadOnly(false);
        ui->AnswerA->setReadOnly(false);
        ui->AnswerB->setReadOnly(false);
        if(ui->AnswerC->isEnabled())
        {
            ui->AnswerC->setReadOnly(false);
        }
        if(ui->AnswerD->isEnabled())
        {
            ui->AnswerD->setReadOnly(false);
        }
        ui->why->setReadOnly(false);
    }
    else
    {
        canChange=false;
        (*tproblem)[tIndex].topic=ui->Topic->toPlainText();
        (*tproblem)[tIndex].option[0]=ui->AnswerA->toPlainText();
        (*tproblem)[tIndex].option[1]=ui->AnswerB->toPlainText();
        if(ui->AnswerC->isEnabled())
        {
            (*tproblem)[tIndex].option[2]=ui->AnswerC->toPlainText();
        }
        if(ui->AnswerD->isEnabled())
        {
            (*tproblem)[tIndex].option[3]=ui->AnswerD->toPlainText();
        }
        (*tproblem)[tIndex].reason=ui->why->toPlainText();
        if(tensorType)
        {
            changeType|=16;
            if(justAnswer!=(*tproblem)[tIndex].answer)
            {
                if((*tproblem)[tIndex].answer==(*tans)[tIndex])
                {
                    changeType|=1;
                }
                else if(justAnswer==(*tans)[tIndex])
                {
                    changeType|=2;
                }
            }
        }
        setMiniInformation();
        DataBase* data=DataBase::get();
        data->updateData((*tproblem)[tIndex],(*tid)[tIndex]);
    }
}

void ShowInformation::chooseId()
{

    if(canChange)
    {
        auto push=qobject_cast<QPushButton*>(sender());
        //Find标志了哪个被选择
        quint8 find=push->whatsThis().toUInt();
        if((*tproblem)[tIndex].answer&16)
        {
            if(((*tproblem)[tIndex].answer&find)==0)
            {
                push->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
                (*tproblem)[tIndex].answer|=find;
            }
            else
            {
                push->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
                (*tproblem)[tIndex].answer&=~find;
            }
        }
        else
        {
            if((*tproblem)[tIndex].answer!=find)
            {
                push->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");

                for(auto child:ui->horizontalFrame->findChildren<QPushButton*>())
                {
                    if(child->whatsThis().toUInt()==(*tproblem)[tIndex].answer)
                    {
                        child->setStyleSheet("border:none;\nbackground-color: rgb(180, 210, 255);");
                    }
                }
                (*tproblem)[tIndex].answer=find;
            }
        }
    }
}

void ShowInformation::setMiniInformation()
{
    if((*tproblem)[tIndex].option.size()>2)
    {
        if((*tproblem)[tIndex].answer&4)
        {
            ui->C->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else if(tensorType&&(*tans)[tIndex]&4)
        {
            ui->C->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
        }
    }
    else
    {
        ui->AnswerC->setEnabled(false);
        ui->C->setEnabled(false);
    }
    if((*tproblem)[tIndex].option.size()>3)
    {
        if((*tproblem)[tIndex].answer&8)
        {
            ui->D->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
        }
        else if(tensorType&&(*tans)[tIndex]&8)
        {
            ui->D->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
        }
    }
    else
    {
        ui->AnswerD->setEnabled(false);
        ui->D->setEnabled(false);
    }
    if((*tproblem)[tIndex].answer&1)
    {
        ui->A->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
    }
    else if(tensorType&&(*tans)[tIndex]&1)
    {
        ui->A->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
    }
    if((*tproblem)[tIndex].answer&2)
    {
        ui->B->setStyleSheet("border:none;\nbackground-color: rgb(0, 255, 0);");
    }else if(tensorType&&(*tans)[tIndex]&2)
    {
        ui->B->setStyleSheet("border:none;\nbackground-color: rgb(255, 0, 0);");
    }
    if((*tRecord)[tIndex]==true)
    {
        ui->pushButton_5->setText("取消收藏");
    }
    else
    {
        ui->pushButton_5->setText("收藏");
    }
    ui->pushButton_3->setText("编辑");
    ui->pushButton_4->setText("返回");
    ui->Topic->setReadOnly(true);
    ui->AnswerA->setReadOnly(true);
    ui->AnswerB->setReadOnly(true);
    if(ui->AnswerC->isEnabled())
    {
        ui->AnswerC->setReadOnly(true);
    }
    if(ui->AnswerD->isEnabled())
    {
        ui->AnswerD->setReadOnly(true);
    }
    ui->why->setReadOnly(true);
}

void ShowInformation::closeEvent(QCloseEvent *event)
{
    emit parentDo(tIndex,changeType);
}

