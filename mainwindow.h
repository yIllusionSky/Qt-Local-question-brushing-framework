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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    QString fileInfo;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setThisInTop(bool choose);
private slots:
    void on_pushButton_2_pressed();


    void on_pushButton_pressed();

    void on_pushButton_5_pressed();


    void on_pushButton_3_pressed();

    void on_pushButton_4_pressed();

    void on_checkBox_4_stateChanged(int arg1);

    void on_pushButton_6_pressed();
    void on_lineEdit_5_editingFinished();

    void on_lineEdit_6_editingFinished();

    void on_pushButton_9_pressed();

    void on_pushButton_7_pressed();

    void on_pushButton_11_pressed();

private:
    Ui::MainWindow *ui;
    void withObject();
    bool state=false;   //这个是个锁
    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
};
#endif // MAINWINDOW_H
