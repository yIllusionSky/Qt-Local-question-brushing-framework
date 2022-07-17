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

#include "filterillusion.h"
#include<QDir>
#include<QFile>
#include<sstream>
#include<QRegularExpression>
filterIllusion::filterIllusion()
{
    this->_It=IllusionFile::get();
}

qsizetype filterIllusion::Deduplication(QString saveName,bool isUSEForce,bool isR)
{
    ProblemStruct problemArray;
    AnswerStruct answerArray;

    QDir saveDir(_It->path[4]);
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
            QString line=in.readLine().trimmed();
            QStringList typeArray=line.split('\t',Qt::SkipEmptyParts);
            if(typeArray.count()!=7)
            {
                continue;
            }
            //0是题目，1-4是选项，5是解析，6是答案
            problemArray.append(typeArray[0]);
            if(typeArray.count()<7)
            {
                typeArray.insert(5,"无解析");
            }
            QPair<quint8,QStringList> onePair;
            onePair.first=typeArray[6].toInt();
            onePair.second.append(typeArray.begin()+1,typeArray.end()-1);

            answerArray.append(onePair);
        }
        oneFile.close();
    }

    for(auto one:saveDir.entryInfoList())
    {
        QFile::remove(one.absoluteFilePath());
    }

    int clearIt;
    qsizetype index=0;

    QFile ThreeFile(_It->path[4]+"\\error.txt");
    if(!ThreeFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return 0;
    }
    QTextStream outs(&ThreeFile);
    QList<QString> removeP;
    removeP.resize(problemArray.size());
    removeP.clear();
    QList<QVector<QString>> removeA;
    removeA.resize(answerArray.size());
    for(qsizetype i=0;i<removeA.size();i++)
    {
        removeP.append(problemArray[i].simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").toUpper());
        removeA[i].resize(4);
        removeA[i].clear();
        for(qsizetype min=0;min<4;min++)
        {
            removeA[i].append(answerArray[i].second[min].simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("、").toUpper());
        }
    }
    if(isR)
    {
        QFile officeFile(_It->path[4]+"\\office.txt");
        QFile windowFile(_It->path[4]+"\\numbernet.txt");
        if(!officeFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            return 0;
        }
        if(!windowFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            return 0;
        }
        QTextStream office(&officeFile);
        QTextStream window(&windowFile);
        for(qsizetype i=1;i<problemArray.count();)
        {
            if(answerArray[i].first>4||answerArray[i].first<1)
            {
                problemArray.removeAt(i);
                answerArray.removeAt(i);
                removeP.removeAt(i);
                removeA.removeAt(i);
                index++;
                continue;
            }
            if(removeP[i].contains("WORD")||removeP[i].contains("PPT")||removeP[i].contains("EXCEL"))
            {
                QString line=problemArray[i]+QString("\t");
                for(int kk=0;kk<5;kk++)
                {
                    line+=answerArray[i].second[kk];
                    line+=QString("\t");
                }

                line+=QString::number(answerArray[i].first);

                //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                office<<line<<Qt::endl;
                problemArray.removeAt(i);
                answerArray.removeAt(i);
                removeP.removeAt(i);
                removeA.removeAt(i);
                index++;
                continue;
            }

            if(removeP[i].contains("IP"))
            {
                int record=0;
                for(qsizetype id=0;id<4;id++)
                {
                    if(removeA[i][id].count('.')==3)
                    {
                        record++;
                    }
                }
                if(record==4)
                {
                    QString line=problemArray[i]+QString("\t");
                    for(int kk=0;kk<5;kk++)
                    {
                        line+=answerArray[i].second[kk];
                        line+=QString("\t");
                    }

                    line+=QString::number(answerArray[i].first);

                    //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                    window<<line<<Qt::endl;
                    problemArray.removeAt(i);
                    answerArray.removeAt(i);
                    removeP.removeAt(i);
                    removeA.removeAt(i);
                    index++;
                    continue;
                }
            }
            if(removeP[i].contains("二进制")+removeP[i].contains("十进制")+removeP[i].contains("八进制")+removeP[i].contains("十六进制")+removeP[i].contains("原码")+removeP[i].contains("真值")
                    +removeP[i].contains("反码")+removeP[i].contains("补码")>=2)
            {
                QString line=problemArray[i]+QString("\t");
                for(int kk=0;kk<5;kk++)
                {
                    line+=answerArray[i].second[kk];
                    line+=QString("\t");
                }

                line+=QString::number(answerArray[i].first);

                //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                window<<line<<Qt::endl;
                problemArray.removeAt(i);
                answerArray.removeAt(i);
                removeP.removeAt(i);
                removeA.removeAt(i);
                index++;
                continue;
            }
            if(removeP[i].contains("GB")+removeP[i].contains("MB"))
            clearIt=1;
            for(qsizetype j=0;j<i;j++)
            {

                //检查不区分大小写
                if(removeP[i].contains(removeP[j])||
                        removeP[j].contains(removeP[i]))
                {
                    //第二层进行判断
                    qsizetype pan=0;
                    for(qsizetype miniI=0;miniI<4;miniI++)
                    {
                        for(qsizetype miniJ=0;miniJ<4;miniJ++)
                        {
                            //对大小写不敏感
                            if(removeA[i][miniI].contains(removeA[j][miniJ])||removeA[j][miniJ].contains(removeA[i][miniI]))
                            {
                                pan++;
                                break;
                            }
                        }
                    }
                    if(pan==4)
                    {
                        if(!removeA[i][answerArray[i].first-1].contains(removeA[j][answerArray[j].first-1])&&!removeA[j][answerArray[j].first-1].contains(removeA[i][answerArray[i].first-1]))
                        {

                            QString line=problemArray[i]+QString("\t");
                            for(int kk=0;kk<5;kk++)
                            {
                                line+=answerArray[i].second[kk];
                                line+=QString("\t");
                            }

                            line+=QString::number(answerArray[i].first);

                            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                            outs<<line<<Qt::endl;

                            line=problemArray[j]+QString("\t");
                            for(int kk=0;kk<5;kk++)
                            {
                                line+=answerArray[j].second[kk];
                                line+=QString("\t");
                            }

                            line+=QString::number(answerArray[j].first);

                            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                            outs<<line<<Qt::endl;
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                            problemArray.removeAt(i-1);
                            answerArray.removeAt(i-1);
                            removeP.removeAt(i-1);
                            removeA.removeAt(i-1);
                            index++;
                            clearIt--;

                        }
                        else if(answerArray[i].second[4].compare("无解析")==0&&answerArray[j].second[4].compare("无解析")!=0)
                        {
                            problemArray.removeAt(i);
                            answerArray.removeAt(i);
                            removeP.removeAt(i);
                            removeA.removeAt(i);
                        }
                        else if(answerArray[i].second[4].compare("无解析")!=0&&answerArray[j].second[4].compare("无解析")==0)
                        {
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                        }
                        else if(removeP[i].length()<=removeP[j].length())
                        {
                            problemArray.removeAt(i);
                            answerArray.removeAt(i);
                            removeP.removeAt(i);
                            removeA.removeAt(i);
                        }
                        else
                        {
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                        }
                        index++;
                        clearIt--;
                        break;
                    }
                }
            }
            i+=clearIt;
        }
        removeP.clear();
        removeA.clear();
        outs.flush();
        ThreeFile.close();

        QFile TwoFile(_It->path[4]+"\\"+saveName);
        //打开失败后的处理
        if(!TwoFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            return 0;
        }

        QTextStream out(&TwoFile);
        for(qsizetype realIndex=0;realIndex<problemArray.count();realIndex++)
        {
            QString line=problemArray[realIndex]+QString("\t");
            for(int kk=0;kk<5;kk++)
            {
                line+=answerArray[realIndex].second[kk];
                line+=QString("\t");
            }

            line+=QString::number(answerArray[realIndex].first);

            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
            out<<line<<Qt::endl;

        }
        out.flush();
        TwoFile.close();
        window.flush();
        windowFile.close();
        office.flush();
        officeFile.close();
    }
    else if(isUSEForce)
    {
        for(qsizetype i=1;i<problemArray.count();)
        {
            if(answerArray[i].first>4||answerArray[i].first<1)
            {
                problemArray.removeAt(i);
                answerArray.removeAt(i);
                removeP.removeAt(i);
                removeA.removeAt(i);
                index++;
                continue;
            }
            clearIt=1;
            for(qsizetype j=0;j<i;j++)
            {

                //检查不区分大小写
                if(removeP[i].contains(removeP[j])||
                        removeP[j].contains(removeP[i]))
                {
                    //第二层进行判断
                    qsizetype pan=0;
                    for(qsizetype miniI=0;miniI<4;miniI++)
                    {
                        for(qsizetype miniJ=0;miniJ<4;miniJ++)
                        {
                            //对大小写不敏感
                            if(removeA[i][miniI].contains(removeA[j][miniJ])||removeA[j][miniJ].contains(removeA[i][miniI]))
                            {
                                pan++;
                                break;
                            }
                        }
                    }
                    if(pan==4)
                    {
                        if(!removeA[i][answerArray[i].first-1].contains(removeA[j][answerArray[j].first-1])&&!removeA[j][answerArray[j].first-1].contains(removeA[i][answerArray[i].first-1]))
                        {

                            QString line=problemArray[i]+QString("\t");
                            for(int kk=0;kk<5;kk++)
                            {
                                line+=answerArray[i].second[kk];
                                line+=QString("\t");
                            }

                            line+=QString::number(answerArray[i].first);

                            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                            outs<<line<<Qt::endl;

                            line=problemArray[j]+QString("\t");
                            for(int kk=0;kk<5;kk++)
                            {
                                line+=answerArray[j].second[kk];
                                line+=QString("\t");
                            }

                            line+=QString::number(answerArray[j].first);

                            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                            outs<<line<<Qt::endl;
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                            problemArray.removeAt(i-1);
                            answerArray.removeAt(i-1);
                            removeP.removeAt(i-1);
                            removeA.removeAt(i-1);
                            index++;
                            clearIt--;

                        }
                        else if(answerArray[i].second[4].compare("无解析")==0&&answerArray[j].second[4].compare("无解析")!=0)
                        {
                            problemArray.removeAt(i);
                            answerArray.removeAt(i);
                            removeP.removeAt(i);
                            removeA.removeAt(i);
                        }
                        else if(answerArray[i].second[4].compare("无解析")!=0&&answerArray[j].second[4].compare("无解析")==0)
                        {
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                        }
                        else if(removeP[i].length()<=removeP[j].length())
                        {
                            problemArray.removeAt(i);
                            answerArray.removeAt(i);
                            removeP.removeAt(i);
                            removeA.removeAt(i);
                        }
                        else
                        {
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                        }
                        index++;
                        clearIt--;
                        break;
                    }
                }
            }
            i+=clearIt;
        }
        removeP.clear();
        removeA.clear();
        outs.flush();

        ThreeFile.close();

        QFile TwoFile(_It->path[4]+"\\"+saveName);
        //打开失败后的处理
        if(!TwoFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            return 0;
        }

        QTextStream out(&TwoFile);
        for(qsizetype realIndex=0;realIndex<problemArray.count();realIndex++)
        {
            QString line=problemArray[realIndex]+QString("\t");
            for(int kk=0;kk<5;kk++)
            {
                line+=answerArray[realIndex].second[kk];
                line+=QString("\t");
            }

            line+=QString::number(answerArray[realIndex].first);

            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
            out<<line<<Qt::endl;

        }
        out.flush();
        TwoFile.close();
    }
    else
    {
        for(qsizetype i=1;i<problemArray.count();)
        {
            if(answerArray[i].first>4||answerArray[i].first<1)
            {
                problemArray.removeAt(i);
                answerArray.removeAt(i);
                removeP.removeAt(i);
                removeA.removeAt(i);
                index++;
                continue;
            }
            clearIt=1;
            for(qsizetype j=0;j<i;j++)
            {
                //检查不区分大小写
                if(removeP[i].contains(removeP[j])||
                        removeP[j].contains(removeP[i]))
                {
                    //第二层进行判断
                    qsizetype pan=0;
                    for(qsizetype miniI=0;miniI<4;miniI++)
                    {
                        for(qsizetype miniJ=0;miniJ<4;miniJ++)
                        {
                            //对大小写不敏感
                            if(removeA[i][miniI].contains(removeA[j][miniJ])||removeA[j][miniJ].contains(removeA[i][miniI]))
                            {
                                pan++;
                                break;
                            }
                        }
                    }
                    if(pan==4)
                    {
                        if(removeA[i][answerArray[i].first-1].compare(removeA[j][answerArray[j].first-1])!=0)
                        {

                            QString line=problemArray[i]+QString("\t");
                            for(int kk=0;kk<5;kk++)
                            {
                                line+=answerArray[i].second[kk];
                                line+=QString("\t");
                            }

                            line+=QString::number(answerArray[i].first);

                            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                            outs<<line<<Qt::endl;

                            line=problemArray[j]+QString("\t");
                            for(int kk=0;kk<5;kk++)
                            {
                                line+=answerArray[j].second[kk];
                                line+=QString("\t");
                            }

                            line+=QString::number(answerArray[j].first);

                            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
                            outs<<line<<Qt::endl;
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                            problemArray.removeAt(i-1);
                            answerArray.removeAt(i-1);
                            removeP.removeAt(i-1);
                            removeA.removeAt(i-1);
                            index++;
                            clearIt--;
                        }
                        else if(answerArray[i].second[4].compare("无解析")==0&&answerArray[j].second[4].compare("无解析")!=0)
                        {
                            problemArray.removeAt(i);
                            answerArray.removeAt(i);
                            removeP.removeAt(i);
                            removeA.removeAt(i);
                        }
                        else if(answerArray[i].second[4].compare("无解析")!=0&&answerArray[j].second[4].compare("无解析")==0)
                        {
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                        }
                        else if(removeP[i].length()<=removeP[j].length())
                        {
                            problemArray.removeAt(i);
                            answerArray.removeAt(i);
                            removeP.removeAt(i);
                            removeA.removeAt(i);
                        }
                        else
                        {
                            problemArray.removeAt(j);
                            answerArray.removeAt(j);
                            removeP.removeAt(j);
                            removeA.removeAt(j);
                        }
                        index++;
                        clearIt--;
                        break;
                    }
                }
            }
            i+=clearIt;
        }
        removeP.clear();
        removeA.clear();
        outs.flush();
        ThreeFile.close();
        QFile TwoFile(_It->path[4]+"\\"+saveName);
        //打开失败后的处理
        if(!TwoFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            return 0;
        }

        QTextStream out(&TwoFile);
        for(qsizetype realIndex=0;realIndex<problemArray.count();realIndex++)
        {
            QString line=problemArray[realIndex]+QString("\t");
            for(int kk=0;kk<5;kk++)
            {
                line+=answerArray[realIndex].second[kk];
                line+=QString("\t");
            }

            line+=QString::number(answerArray[realIndex].first);

            //0是题目，1-4是选项，5是解析，6是答案(答案用1,2,3,4)
            out<<line<<Qt::endl;

        }
        out.flush();
        TwoFile.close();
    }
    return index;
}

void filterIllusion::MergeFile(QString regStr,QString findStr,QString shuffle,QString saveName,bool isRemove,bool isNum)
{
    QVector<quint16> type;
    type.append(shuffle.indexOf(QChar('p'),0,Qt::CaseInsensitive));
    type.append(shuffle.indexOf(QChar('A'),0,Qt::CaseInsensitive));
    type.append(shuffle.indexOf(QChar('B'),0,Qt::CaseInsensitive));
    type.append(shuffle.indexOf(QChar('C'),0,Qt::CaseInsensitive));
    type.append(shuffle.indexOf(QChar('D'),0,Qt::CaseInsensitive));
    type.append(shuffle.indexOf(QChar('r'),0,Qt::CaseInsensitive));
    type.append(shuffle.indexOf(QChar('O'),0,Qt::CaseInsensitive));
    QDir saveDir(_It->path[4]);
    saveDir.setFilter(QDir::Files);
    saveDir.setNameFilters(QStringList("*.txt"));
    saveDir.setSorting(QDir::Name);

    QStringList qGetAnswer;


    for(auto one:saveDir.entryInfoList())
    {
        QFile oneFile(one.absoluteFilePath());

        //打开失败后的处理
        if(!oneFile.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            continue;
        }

        QTextStream in(&oneFile);
        QRegularExpression reg(regStr);

        QString readText;
        while(!in.atEnd())
        {
            readText.append(in.readLine().trimmed());
            readText.append("\n");

        }
        reg.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatchIterator regl= reg.globalMatch(readText,0);
        while(regl.hasNext())
        {

            QRegularExpressionMatch match=regl.next();
            qGetAnswer.append(match.captured());
        }
        oneFile.close();
        oneFile.remove();

    }


    QFile twoFile(_It->path[4]+"\\"+saveName);
    QTextStream out(&twoFile);
    if(!twoFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    QRegularExpression reg2(findStr);
    reg2.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
    for(qsizetype iss=0;iss<qGetAnswer.count();iss++)
    {
        QRegularExpressionMatch match2=reg2.match(qGetAnswer[iss]);
        QString qdd[7];
        QStringList man;

        if(match2.hasMatch())
        {
            for(int fIndex=1;fIndex<=match2.lastCapturedIndex();fIndex++)
            {

                if(isRemove&&type.indexOf(fIndex-1)>0&&type.indexOf(fIndex-1)<5)
                {
                    qdd[type.indexOf(fIndex-1)]=match2.captured(fIndex).mid(2);

                }
                else if(isNum&&type.indexOf(fIndex-1)==5)
                {
                    qdd[type.indexOf(fIndex-1)]=QString(QChar(match2.captured(fIndex).toStdString().c_str()[0]-'A'+'1'));
                }
                else
                {
                    qdd[type.indexOf(fIndex-1)]=match2.captured(fIndex);

                }
            }

        }
        for(int idd=0;idd<5;idd++)
        {
            man.append(qdd[idd]);
        }
        if(qdd[6].isEmpty())
        {
            man.append("缺省");
        }
        else
        {
            man.append(qdd[6]);
        }
        man.append(qdd[5]);

        out<<man.join("\t")<<Qt::endl;
    }
    out.flush();
    twoFile.close();
    return;
}

