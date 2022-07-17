#include "mydialog.h"
#include"ui_dialog.h"
#include<QHostInfo>
#include<QMessageBox>
#include<ctime>
#define PASS_FIRST 0x35
#define PASS_SECOND 0x6b
#define PASS_THIRD 0x28
#define MY_PORT 14668
#define WAIT_MS 2000
//关闭服务器的关键词：Shellhx
mydialog::mydialog(QWidget* parent,QString ipInfo,int portInfo):QDialog(parent),ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ip=ipInfo;
    port=portInfo;
    mSocket=new QUdpSocket;
    connect(mSocket,&QUdpSocket::readyRead,this,&mydialog::readlater);
}

mydialog::~mydialog()
{
    if(mSocket->isOpen())
    {
        mSocket->close();
    }
    mSocket->close();
    disconnect(mSocket,&QUdpSocket::readyRead,this,&mydialog::readlater);
    delete mSocket;
    delete ui;
}

void mydialog::on_pushButton_pressed()
{
    QByteArray byte=ui->comboBox->currentText().toUtf8();
    byte.append(ui->textEdit->toPlainText().toUtf8());
    quint32 qt=byte.size();

    time_t nowTime=time(NULL);
    q1=qt%255;                          //Q1对长度进行加密
    q1^=PASS_FIRST;
    q2=nowTime/60%255;               //Q2利用时间加密(分钟)
    q2^=PASS_SECOND;
    q3=nowTime/3600%255;                    //Q3利用时间加密(小时)
    q3^=PASS_THIRD;

    byte.insert(0,q1);
    byte.insert(0,q2);
    byte.insert(0,q3);
    if(byte.size()<=512&&byte.size()>6)
    {
        if(mSocket->bind(MY_PORT,QAbstractSocket::DontShareAddress))
        {
            inputsize=mSocket->writeDatagram(byte,QHostAddress(ip),port);
            if(inputsize!=-1)
            {
                ui->pushButton->setEnabled(false);
                ui->comboBox->setEnabled(false);
                ui->textEdit->setEnabled(false);
                return;
            }
            else
            {
                QMessageBox::information(this,"失败","无法发送数据包");
            }
        }
        else
        {
            QMessageBox::information(this,"失败","无法绑定客户端");
        }
    }
    else
    {
        QMessageBox::information(this,"失败","数据太大或太小");
    }
    if(mSocket->isOpen())
    {
        mSocket->close();
    }
    this->close();
    return;
}

void mydialog::readlater()
{

    QByteArray MyDate;
    while(mSocket->hasPendingDatagrams())
    {

    MyDate.resize(mSocket->pendingDatagramSize());
    qsizetype result=mSocket->readDatagram(MyDate.data(),MyDate.size());//读取数据
    if(result==3)
    {
        if((uchar)MyDate.data()[0]==q3&&(uchar)MyDate.data()[1]==q2&&(uchar)MyDate.data()[2]==q1)
        {

            QMessageBox::information(this,"成功",QString("发送了")+QString::number(inputsize)+"个字节的数据");
            mSocket->close();
            this->close();
            break;
        }
    }
    }
}

