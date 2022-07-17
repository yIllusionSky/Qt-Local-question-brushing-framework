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

#ifndef REASON_H
#define REASON_H

#include <QWidget>
#include"database.h"
#include<QStandardItem>
namespace Ui {
class reason;
}
class ShowInformation;
class MyReason : public QWidget
{
    Q_OBJECT
public:
    explicit MyReason(QWidget* parent,quint32 times,QList<quint8>* chooseAnswer,QList<ProblemData>* problem,QList<quint32>* problemId);
    ~MyReason();
signals:
    void closeParent();
private:
    Ui::reason *ui;
    QStandardItemModel* model;
    QList<quint8>* myChooseAnswer;
    QList<ProblemData>* myProblem;
    QList<quint32>* myProblemId;
    QList<bool> isRecord;
    ShowInformation* myInformation;
    quint32 score;
    // QWidget interface
private slots:
    void on_topicView_doubleClicked(const QModelIndex &index);
    void sandDo(quint32 theIndex,quint8 whatHappen);

    // QWidget interface
    void on_pushButton_pressed();


    void on_checkBox_stateChanged(int arg1);

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // REASON_H
