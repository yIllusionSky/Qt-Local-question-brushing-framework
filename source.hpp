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

#ifndef SOURCE_HPP
#define SOURCE_HPP
#include<QString>
#include<QStringList>
#include<QPair>

using AnswerT=QPair<quint8,QStringList>;
using AnswerStruct=QList<AnswerT>;
using ProblemStruct=QStringList;

class IllusionFile;
class filterIllusion;
class searchWidget;
//该类在全局中只允许存在一个
class IllusionFile
{

public:

    /*创建类的指针，如果类不存在则创建*/
    static IllusionFile *get();
    static void create(QStringList& fileList);
    /**
     * @brief searchAnswer  搜索答案
     * @param problem       需要搜索的问题，多段匹配用空格隔开(传一个对象直接获得结果)
     * @param answer        返回的结果，一个集合
     */
    void searchAnswer(QStringList& send,ProblemStruct& problem,AnswerStruct& answer,int type=0);


    /**
     * @brief getProblem    获取随机50道题目
     * @param problem       获取的题目
     * @param answer        获取题目对应的答案
     * @param whichType     选择题目类型(哪个模块)[如果设-1，则在全部题目中随机]
     */
    void getProblem(ProblemStruct& problem,AnswerStruct& answer,qsizetype count,bool isContinue,qsizetype continueIndex);
    void getErrorProblem(std::vector<int>& errorIndex,ProblemStruct& problem,AnswerStruct& answer,qsizetype count);
    void saveScore(std::vector<int>& errorIndex,std::vector<int>& typeIndex);
    void WriteErrorAnswer(ProblemStruct& problem,AnswerStruct& answer);

    /*加载|更新错误*/
    void SaveError();
    void LoadError();
    qsizetype GetErrorCount();
    qsizetype GetProblemCount();
    //它的朋友
    friend class filterIllusion;
    friend class searchWidget;

public slots:
    static void release();

private:

    /*锁*/
    static qsizetype _Clock;

    /*存储自己*/
    static IllusionFile* _Self;

    /*初始化类*/
    IllusionFile();
    IllusionFile(QStringList& qs);

    /*析构函数*/
    ~IllusionFile();


    /*Mac地址，用来验证*/
    QString macAddress;

    /*存储题目和答案，以及正确的序号*/
    ProblemStruct problemArray;
    AnswerStruct answerArray;

    /*存储错误相关的属性*/
    ProblemStruct problemArrayE;
    AnswerStruct answerArrayE;
    QVector<int> Ecount;

    /* 共4个Path
     * path[0]是主目录位置
     * path[1]是Save文件位置(存放主文件位置)
     * path[2]是Error文件位置(存放错误文件位置)
     * path[3]是Load文件位置(正则表达式存放)
     * path[4]是Temp文件位置(将临时文件进行压缩或处理成最终文件)
     * (暂无)path[5]是Index索引文件位置(快速查找和删除)
     */
    QString path[5];
};

#endif // SOURCE_HPP
