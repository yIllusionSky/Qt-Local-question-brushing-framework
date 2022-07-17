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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<problemfile.h>
#include<QFileDialog>
#include<searchwidget.h>
#include<QInputDialog>
#include<config.h>
#include<file.h>
#include<createdb.h>
#include<QDir>
#include<QMessageBox>
#include"editclass.h"
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDir dirss;
    this->setFixedSize(450,660);
    if(!dirss.exists(QApplication::applicationDirPath()+"/Save/")) dirss.mkdir(QApplication::applicationDirPath()+"/Save/");
    fileInfo="!!!default";
    DataBase::setSavePath(QApplication::applicationDirPath()+"/Save/");
    if(QFile::exists(QApplication::applicationDirPath()+"/Save/"+fileInfo))
    {
        DataBase::get(fileInfo);
    }
    else
    {
        QDir qdir(QApplication::applicationDirPath()+"/Save");
        QStringList filters;
        filters<<"*.db";
        qdir.setNameFilters(filters);
        if(qdir.count()==0)
        {
            DataBase::get();
            this->fileInfo="!!!default";
        }
        else
        {
            this->fileInfo=qdir.entryInfoList().at(0).baseName();
            DataBase::get(this->fileInfo);
        }

    }
}

MainWindow::~MainWindow()
{
    DataBase::release();
    delete ui;

}


void MainWindow::on_pushButton_2_pressed()
{
    if(state==true) return;
    state=true;
    file *q=new file(this);
    connect(q,&file::parentDo,this,&MainWindow::withObject);
    q->show();
    q->raise();
    q->activateWindow();
}

void MainWindow::setThisInTop(bool choose)
{
    if(choose) //置顶
    {
        auto m_flags=windowFlags();
        setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    }
    else    //取消置顶
    {
        auto m_flags=windowFlags();
        m_flags&=~Qt::WindowStaysOnTopHint;
        setWindowFlags(m_flags);
    }
    show();
}

void MainWindow::on_pushButton_pressed()
{
    if(state==true) return;
    DataBase* data=DataBase::get();
    quint32 count;
    data->getDataCount(&count);
    if(count<data->selectIndexFromUInt(InfoType::OneCount))
    {
        return;
    }
    state=true;
    problemFile* q=new problemFile(this,TopicEnum::Topic);
    connect(q,&problemFile::parentDo,this,&MainWindow::withObject);
    q->show();
    q->raise();
    q->activateWindow();
}


void MainWindow::on_pushButton_5_pressed()
{
    if(state==true) return;
    state=true;
    createDB* cdb=new createDB(this);
    connect(cdb,&createDB::parentDo,this,&MainWindow::withObject);
    cdb->show();
    cdb->raise();
    cdb->activateWindow();
}

void MainWindow::on_pushButton_3_pressed()
{
    if(state==true) return;
    DataBase* data=DataBase::get();
    quint32 count;
    data->getDataCount(nullptr,nullptr,&count);
    if(count<data->selectIndexFromUInt(InfoType::OneCount))
    {
        return;
    }
    state=true;
    problemFile* q=new problemFile(this,TopicEnum::RecordTopic);
    connect(q,&problemFile::parentDo,this,&MainWindow::withObject);
    q->show();
    q->raise();
    q->activateWindow();
}

void MainWindow::showEvent(QShowEvent *event)
{
    state=false;
    quint32 errorCount,topicCount,loveCount;
    auto data=DataBase::get();
    data->getDataCount(&topicCount,&errorCount,&loveCount);
    QString textInfo;
    textInfo.resize(2048);
    textInfo.clear();
    textInfo.append("<font color=\"DarkCyan\">[名称]</font><font color=\"Navy\">");
    textInfo.append(data->selectInfo(1));
    textInfo.append("</font><br><font color=\"Indigo\">[作者]</font><font color=\"LimeGreen\">");
    textInfo.append(data->selectInfo(2));
    textInfo.append("</font><br><font color=\"MediumOrchid\">[题量|错题|收藏]</font><font color=\"DarkGreen\">");
    textInfo.append(QString::number(topicCount)+"|"+QString::number(errorCount)+"|"+QString::number(loveCount));
    textInfo.append("</font><br><font color=\"Orange\">[累计做题]</font><font color=\"Tomato\">");
    textInfo.append(QString::number(data->selectIndexFromUInt(InfoType::Count)));
    textInfo.append("</font><br><br><font color=\"Grey\">");
    textInfo.append(data->selectInfo(5));
    textInfo.append("</font>");
    ui->textEdit->setHtml(textInfo);

    this->ui->lineEdit_5->setText(QString::number(data->selectIndexFromUInt(InfoType::OneCount)));
    this->ui->lineEdit_6->setText(QString::number(data->selectIndexFromUInt(InfoType::Index)));

}


void MainWindow::on_pushButton_4_pressed()
{
    if(state==true) return;
    state=true;
    searchWidget* q=new searchWidget(this);
    connect(q,&searchWidget::parentDo,this,&MainWindow::withObject);
    q->show();
    q->raise();
    q->activateWindow();
}


void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    setThisInTop(Qt::CheckState::Checked==arg1);
}


void MainWindow::on_pushButton_6_pressed()
{
    if(state==true) return;
    DataBase* data=DataBase::get();
    quint32 count;
    data->getDataCount(&count);
    if(count<data->selectIndexFromUInt(InfoType::OneCount)+data->selectIndexFromUInt(InfoType::Index))
    {
        return;
    }
    state=true;
    problemFile* q=new problemFile(this,TopicEnum::Continue);
    connect(q,&problemFile::parentDo,this,&MainWindow::withObject);
    q->show();
    q->raise();
    q->activateWindow();
}


void MainWindow::on_lineEdit_5_editingFinished()
{
    DataBase* data=DataBase::get();
    data->updateIndexFromUInt(ui->lineEdit_5->text().toUInt(),InfoType::OneCount);
}


void MainWindow::on_lineEdit_6_editingFinished()
{
    DataBase* data=DataBase::get();
    data->updateIndexFromUInt(ui->lineEdit_6->text().toUInt(),InfoType::Index);
}


void MainWindow::on_pushButton_9_pressed()
{
    if(state==true) return;
    DataBase* data=DataBase::get();
    quint32 count;
    data->getDataCount(nullptr,&count);     //获取错误的个数
    if(count<data->selectIndexFromUInt(InfoType::OneCount))
    {
        return;
    }
    state=true;
    problemFile* q=new problemFile(this,TopicEnum::ErrorTopic);
    connect(q,&problemFile::parentDo,this,&MainWindow::withObject);
    q->show();
    q->raise();
    q->activateWindow();
}

void MainWindow::withObject()
{
    delete sender();
    this->showEvent(nullptr);
}


void MainWindow::on_pushButton_7_pressed()
{
    DataBase* data=DataBase::get();
    data->deleteData();
    this->showEvent(nullptr);
    QMessageBox::information(this,"信息","处理成功");
    DataBase::get();
}


void MainWindow::on_pushButton_11_pressed()
{
    if(state==true) return;
    state=true;
    EditClass* q=new EditClass(this);
    connect(q,&EditClass::parentDo,this,&MainWindow::withObject);
    q->show();
    q->raise();
    q->activateWindow();
}

