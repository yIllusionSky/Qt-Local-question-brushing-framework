#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include<QUdpSocket>
namespace Ui {
    class Dialog;
}
class mydialog : public QDialog
{
    Q_OBJECT
public:
    mydialog(QWidget* parent,QString ipInfo,int portInfo);
    ~mydialog();
private slots:
    void on_pushButton_pressed();
    void readlater();
private:
    qsizetype inputsize;
    Ui::Dialog *ui;
    QString ip;
    QUdpSocket* mSocket;
    uchar q1,q2,q3;
    int port;
};

#endif // MYDIALOG_H
