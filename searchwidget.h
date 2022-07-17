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

#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include<QTextEdit>
#include<QStandardItemModel>
#include"database.h"
#include"source.hpp"
#define MORE_THREAD_COUNT 24
namespace Ui
{
class searchWidget;
}
class ShowInformation;
class searchWidget : public QWidget
{
    Q_OBJECT
public:
    friend class IllusionFile;
    explicit searchWidget(QWidget *parent = nullptr);
    ~searchWidget();

    qsizetype searchIndex;
signals:
    void parentDo();
private slots:

    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

    void on_topicView_doubleClicked(const QModelIndex &index);
    void sandDo(quint32 theIndex,quint8 whatHappen);
private:
    Ui::searchWidget *ui;

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


#endif // SEARCHWIDGET_H
