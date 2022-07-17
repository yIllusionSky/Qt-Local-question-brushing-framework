#ifndef EDITCLASS_H
#define EDITCLASS_H

#include <QWidget>
#include<QStandardItem>
#include"database.h"
namespace Ui {
class EditClass;
}
class ShowInformation;
class EditClass : public QWidget
{
    Q_OBJECT

public:
    explicit EditClass(QWidget *parent = nullptr);
    ~EditClass();
signals:
    void parentDo();

private slots:
    void on_topicView_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_pressed();
    void sandDo(quint32 theIndex,quint8 whatHappen);

    void on_pushButton_3_pressed();

private:
    Ui::EditClass *ui;
    QStandardItemModel* model;
    QList<ProblemData> myProblem;
    QList<quint32> myProblemId;
    QList<bool> isRecord;
    QList<bool> errorRecord;
    ShowInformation* myInformation;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // EDITCLASS_H
