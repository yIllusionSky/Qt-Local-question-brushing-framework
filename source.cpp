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

#include"source.hpp"
#include<QDir>
#include<QFile>
#include<QApplication>
#include<sstream>
#include<random>
const int MAX_ERROR_COUNT=4;

qsizetype IllusionFile::_Clock=0;
IllusionFile* IllusionFile::_Self=nullptr;

IllusionFile *IllusionFile::get()
{
    if(IllusionFile::_Self==nullptr)
    {
        IllusionFile::_Self=new IllusionFile();

    }
    return IllusionFile::_Self;
}

void IllusionFile::create(QStringList& fileList)
{
    if(IllusionFile::_Self!=nullptr)
    {
        release();
    }
    if(fileList.isEmpty())
    {
        IllusionFile::_Self=new IllusionFile();
    }
    else
    {
        IllusionFile::_Self=new IllusionFile(fileList);
    }
}

void IllusionFile::searchAnswer(QStringList& send,ProblemStruct &problem, AnswerStruct &answer, int type)
{
    if(type==0)
    {
        for(qsizetype index=0;index<problemArray.count();index++)
        {
            bool isContinue=false;
            for(int indexB=0;indexB<send.count();indexB++)
            {
                if(!problemArray[index].contains(send[indexB],Qt::CaseInsensitive))
                {
                    isContinue=true;
                    break;
                }
            }
            if(isContinue) continue;
            problem.append(problemArray[index]);
            answer.append(answerArray[index]);
        }
    }
    else
    {
        for(qsizetype index=0;index<problemArray.count();index++)
        {
            bool isSure=true;
            for(qsizetype miniAnswer=0;miniAnswer<4;miniAnswer++)
            {
                bool isContinue=false;
                for(int indexB=0;indexB<send.count();indexB++)
                {
                    if(!answerArray[index].second[miniAnswer].contains(send[indexB],Qt::CaseInsensitive))
                    {
                        isContinue=true;
                        break;
                    }
                }
                if(isContinue) continue;
                isSure=false;
                break;
            }
            if(isSure) continue;
            problem.append(problemArray[index]);
            answer.append(answerArray[index]);
        }
    }
}


void IllusionFile::getProblem(ProblemStruct &problem, AnswerStruct &answer,qsizetype count,bool isContinue,qsizetype continueIndex)
{
    if(isContinue)
    {
        if(count+continueIndex>answerArray.count()) return;
        for(qsizetype i=0;i<count;i++)
        {
            problem.append(this->problemArray[i+continueIndex]);
            answer.append(this->answerArray[i+continueIndex]);
        }
    }
    else
    {
        if(count>answerArray.count()) return ;
        std::vector<qsizetype> pos;
        for(qsizetype i=0;i<this->answerArray.count();i++)
        {
            pos.push_back(i);
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);
        std::shuffle(pos.begin(), pos.end(), g);
        for(qsizetype i=0;i<count;i++)
        {
            problem.append(this->problemArray[pos[i]]);
            answer.append(this->answerArray[pos[i]]);
        }
    }
}

void IllusionFile::getErrorProblem(std::vector<int> &errorIndex, ProblemStruct &problem, AnswerStruct &answer, qsizetype count)
{
    std::vector<qsizetype> pos;
    if(count>answerArrayE.count()) return;

    for(qsizetype i=0;i<this->answerArrayE.count();i++)
    {
        pos.push_back(i);
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 g(seed);
    std::shuffle(pos.begin(), pos.end(), g);
    for(qsizetype i=0;i<count;i++)
    {
        errorIndex.push_back(pos[i]);
        problem.append(this->problemArrayE[pos[i]]);
        answer.append(this->answerArrayE[pos[i]]);
    }
}

void IllusionFile::saveScore(std::vector<int> &errorIndex,std::vector<int> &typeIndex)
{
    for(int i=0;i<errorIndex.size();i++)
    {
        if(typeIndex.at(i)==0)
        {
            this->Ecount[errorIndex[i]]--;
        }
        else
        {
            this->Ecount[errorIndex[i]]=MAX_ERROR_COUNT;
        }
    }
    for(int i=this->problemArrayE.count()-1;i>=0;i--)
    {
        if(this->Ecount[i]==0)
        {
            this->Ecount.removeAt(i);
            this->problemArrayE.removeAt(i);
            this->answerArrayE.removeAt(i);
        }
    }
}

void IllusionFile::WriteErrorAnswer(ProblemStruct &problem, AnswerStruct &answer)
{
    qsizetype Msize=answerArrayE.count();
    for(qsizetype realIndex=0;realIndex<problem.count();realIndex++)
    {
        bool isContinue=false;
        for(qsizetype shu=0;shu<Msize;shu++)
        {
            if(problem[realIndex].compare(problemArrayE[shu])==0)
            {
                int isBreak=true;
                for(int mini=0;mini<4;mini++)
                {
                    if(answer[realIndex].second[mini].compare(answerArrayE[shu].second[mini])!=0)
                    {
                        isBreak=false;
                    }
                }
                if(isBreak)
                {
                    this->Ecount[shu]=MAX_ERROR_COUNT;
                    isContinue=true;
                    break;
                }
            }
        }
        if(isContinue) continue;
        this->answerArrayE.append(answer[realIndex]);
        this->problemArrayE.append(problem[realIndex]);
        this->Ecount.append(MAX_ERROR_COUNT);
    }
}

void IllusionFile::release()
{
    if(IllusionFile::_Self)
    {
        delete IllusionFile::_Self;
        IllusionFile::_Self=nullptr;
    }
}

IllusionFile::IllusionFile()
{

    /*第一步，载入所有文件夹的位置*/
    this->path[0]=QApplication::applicationDirPath();
    this->path[1]=this->path[0]+("/Save");
    this->path[2]=this->path[0]+("/Error");
    this->path[3]=this->path[0]+("/Load");
    this->path[4]=this->path[0]+("/Temp");

    /*第二步，载入题目文件和答案文件*/
    QDir saveDir(this->path[1]);
    saveDir.setFilter(QDir::Files);
    saveDir.setNameFilters(QStringList("*.txt"));
    saveDir.setSorting(QDir::Name);


    for(auto one:saveDir.entryInfoList())
    {
        QFile oneFile(one.absoluteFilePath());

        //打开失败后的处理
        if(!oneFile.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            continue;
        }

        QTextStream in(&oneFile);
        while(!in.atEnd())
        {
            QString line=in.readLine();
            line=line.trimmed();
            QStringList typeArray=line.split('\t',Qt::SkipEmptyParts);

            //0是题目，1-4是选项，5是解析，6是答案

            if(typeArray.count()<7)
            {
                continue;
            }
            this->problemArray.append(typeArray[0]);
            QPair<quint8,QStringList> onePair;
            onePair.first=typeArray[6].toInt();
            onePair.second.append(typeArray.begin()+1,typeArray.end()-1);

            this->answerArray.append(onePair);
        }

        oneFile.close();
    }
    this->LoadError();
}

IllusionFile::IllusionFile(QStringList &qs)
{
    /*第一步，载入所有文件夹的位置*/
    this->path[0]=QApplication::applicationDirPath();
    this->path[1]=this->path[0]+("/Save");
    this->path[2]=this->path[0]+("/Error");
    this->path[3]=this->path[0]+("/Load");
    this->path[4]=this->path[0]+("/Temp");

    /*第二步，载入题目文件和答案文件*/

    for(auto one:qs)
    {
        QFile oneFile(one);

        //打开失败后的处理
        if(!oneFile.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            continue;
        }

        QTextStream in(&oneFile);
        while(!in.atEnd())
        {
            QString line=in.readLine();
            line=line.trimmed();
            QStringList typeArray=line.split('\t',Qt::SkipEmptyParts);

            //0是题目，1-4是选项，5是解析，6是答案

            if(typeArray.count()!=7)
            {
                continue;
            }
            this->problemArray.append(typeArray[0]);
            QPair<quint8,QStringList> onePair;
            onePair.first=typeArray[6].toInt();
            onePair.second.append(typeArray.begin()+1,typeArray.end()-1);

            this->answerArray.append(onePair);
        }

        oneFile.close();
    }
    this->LoadError();
}

IllusionFile::~IllusionFile()
{
    //清理内存
    this->SaveError();
    this->answerArray.clear();
    this->problemArray.clear();
    this->answerArrayE.clear();
    this->problemArrayE.clear();
    this->Ecount.clear();
}

void IllusionFile::SaveError()
{
    if(!this->answerArrayE.isEmpty())
    {
        QFile::remove(path[2]+"\\"+"ERROR.txt");
        /*写入操作*/
        QFile TwoFile(path[2]+"\\"+"ERROR.txt");
        //打开失败后的处理
        if(!TwoFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            return;
        }
        QTextStream out(&TwoFile);
        for(qsizetype realIndex=0;realIndex<problemArrayE.count();realIndex++)
        {
            QString line=problemArrayE[realIndex]+QString("\t");
            for(int kk=0;kk<5;kk++)
            {
                line+=answerArrayE[realIndex].second[kk];
                line+=QString("\t");
            }

            line+=QString::number(answerArrayE[realIndex].first);
            line+=QString("\t");
            line+=QString::number(Ecount[realIndex]);
            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)，7是次数
            out<<line<<Qt::endl;

        }
        out.flush();
        TwoFile.close();

        this->answerArrayE.clear();
        this->problemArrayE.clear();
        this->Ecount.clear();
    }
}

void IllusionFile::LoadError()
{
    if(this->answerArrayE.isEmpty())
    {

        QFile oneFile(path[2]+"\\"+"ERROR.txt");

        //打开失败后的处理
        if(!oneFile.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            return;
        }

        QTextStream in(&oneFile);
        while(!in.atEnd())
        {
            QString line=in.readLine().trimmed();

            QStringList typeArray=line.split('\t',Qt::SkipEmptyParts);

            //0是题目，1-4是选项，5是解析，6是答案，7是次数

            if(typeArray.count()!=8)
            {
                continue;
            }
            this->problemArrayE.append(typeArray[0]);
            QPair<quint8,QStringList> onePair;
            onePair.first=typeArray[6].toInt();
            onePair.second.append(typeArray.begin()+1,typeArray.end()-2);
            this->Ecount.append(typeArray[7].toInt());
            this->answerArrayE.append(onePair);

        }
    }
}

qsizetype IllusionFile::GetErrorCount()
{
    return this->Ecount.count();
}

qsizetype IllusionFile::GetProblemCount()
{
    return this->problemArray.size();
}
