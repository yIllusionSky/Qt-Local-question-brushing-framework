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

#ifndef PROBLEMFILE_H
#define PROBLEMFILE_H

#include <QWidget>
#include<QButtonGroup>
#include<database.h>
#include<QTime>

namespace Ui {
    class TestWindow;
}
enum class TopicEnum:quint8
{
    Topic=1,
    ErrorTopic=2,
    RecordTopic=4,
    Continue=8,
    Random=16,
    Show=32
};

class problemFile : public QWidget
{
    Q_OBJECT
public:
    explicit problemFile(QWidget *parent,TopicEnum enumtype=TopicEnum::Topic);
    ~problemFile();
    TopicEnum choose;
signals:
    void parentDo();
private slots:
    void on_pushButton_2_pressed();
    void chooseID();
    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_3_pressed();

    void on_pushButton_4_pressed();
    void updateTimer();
    void freeType();
private:
    void setIndex(quint32 theIndex);
    Ui::TestWindow* ui;
    quint32 index=0;
    QTimer *timer;
    QList<quint8> chooseAnswer;
    QList<ProblemData> problem;
    QList<quint32> problemId;

    bool autoNext;
    quint32 answerCount;
    quint32 second;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
};
#endif // PROBLEMFILE_H
