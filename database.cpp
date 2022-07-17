/*注意第一行是字段：题目,A,B,C,D,答案,解析,多选*/
#include "database.h"
#include <QCoreApplication>
#include <QDebug>
#include <QSqlRecord>
#include<QDir>
#include<QFile>
#include<QAxObject>
DataBase* DataBase::_Self=nullptr;
QString DataBase::dataBaseName=nullptr;
QString DataBase::qInfoDirPath=nullptr;
DataBase::DataBase(QString name, QObject *parent)
    : QObject(parent)
{

    if (QSqlDatabase::contains(name)) {

        m_DataBase = QSqlDatabase::database(name);

    }
    else {

        if(m_DataBase.isOpen())
        {
            uninitTable();
        }
        m_DataBase = QSqlDatabase::addDatabase("QSQLITE",name);
        m_DataBase.setDatabaseName(qInfoDirPath+name+".db");
        if(!QFileInfo::exists(qInfoDirPath+name+".db"))
        {
            initTable();
        }
        else
        {
            m_DataBase.open();
        }
    }

    dataName=name;
    QSqlQuery query(m_DataBase);
    if(!query.exec("PRAGMA foreign_keys = ON;"))
    {
        QSqlError sqlerror = query.lastError();
        QString texterr = sqlerror.text();
    }

}

DataBase::~DataBase()
{
    uninitTable();
}



bool DataBase::initTable()
{
    if (!m_DataBase.open()) {
        return false;
    }

    //打开数据库
    //创建数据表realdata

    if(!isExistTable("tProblem"))
    {
        createTable("tProblem");
    }
    if(!isExistTable("tProblemError"))
    {
        createTable("tProblemError");
    }
    if(!isExistTable("tProblemRecord"))
    {
        createTable("tProblemRecord");
    }
    if(!isExistTable("tProblemSelect"))
    {
        createTable("tProblemSelect");
    }
    if(!isExistTable("tInfo"))
    {
        createTable("tInfo");
    }
    //    QSqlQuery query(m_DataBase);
    //    query.prepare(QString("DELETE FROM tProblem WHERE id='%1'").arg("1"));
    //    query.exec();
    //    query.prepare(QString("SELECT * FROM tProblemError"));
    //    query.exec();
    //    query.next();
    //    qDebug()<<query.value(0).toUInt();
    return true;
}

bool DataBase::uninitTable()
{
    m_DataBase.close();
    m_DataBase=QSqlDatabase();
    QSqlDatabase::removeDatabase(dataName);
    return true;
}

bool DataBase::isExistTable(QString table)
{
    bool bRet = false;

    QSqlQuery query(m_DataBase);
    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));    //关键的判断
    if (query.next())
    {
        if (query.value(0).toUInt() > 0)
        {
            bRet = true;
        }
    }
    return bRet;
}

bool DataBase::createTable(QString table)
{
    bool success=false;

    QSqlQuery query(m_DataBase);
    if(!query.exec("PRAGMA foreign_keys = ON;"))
    {
        QSqlError sqlerror = query.lastError();
        QString texterr = sqlerror.text();
        return false;
    }
    if (table == QString("tProblem")) {

        success = query.exec("CREATE TABLE tProblem ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "topic NVARCHAR(255) NOT NULL, "
                             "A NVARCHAR(255) NOT NULL, "
                             "B NVARCHAR(255) NOT NULL, "
                             "C NVARCHAR(255), "
                             "D NVARCHAR(255), "
                             "reason NVARCHAR(255) NOT NULL,"
                             "answer tinyint unsigned NOT NULL)");
    }
    else if(table==QString("tProblemError"))
    {
        success = query.exec("CREATE TABLE tProblemError ("
                             "id INTEGER PRIMARY KEY REFERENCES tProblem(id) ON DELETE cascade, "
                             "errorCount tinyint unsigned NOT NULL)");
    }
    else if(table==QString("tProblemRecord"))
    {
        success = query.exec("CREATE TABLE tProblemRecord ("
                             "id INTEGER PRIMARY KEY REFERENCES tProblem(id) ON DELETE cascade)");
    }
    else if(table==QString("tProblemSelect"))
    {
        success = query.exec("CREATE TABLE tProblemSelect ("
                             "id INTEGER PRIMARY KEY REFERENCES tProblem(id) ON DELETE cascade, "
                             "topic NVARCHAR(255) NOT NULL, "
                             "A NVARCHAR(255) NOT NULL, "
                             "B NVARCHAR(255) NOT NULL, "
                             "C NVARCHAR(255), "
                             "D NVARCHAR(255))");
    }
    else if(table==QString("tInfo"))
    {
        success=query.exec("CREATE TABLE tInfo ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "title NCHAR(10) NOT NULL, "
                           "info NVARCHAR(255))");
    }
    else{

    }
    if (success) {
        return true;   //新建数据库成功
    }
    else {
        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        return false;
    }
}

DataBase *DataBase::get(QString qr)
{
    if(DataBase::_Self==nullptr)
    {
        if(qr==nullptr)
        {
            if(DataBase::dataBaseName==nullptr)
            {
                DataBase::dataBaseName="!!!default";
                DataBase::_Self=new DataBase(DataBase::dataBaseName);
                auto loadInfo=QList<QString>({"默认","IllusionAutumn","127.0.0.1","9427","这个文件是初始化时默认自带的文件"});
                DataBase::_Self->initInfo(loadInfo,50);
            }
            else
            {
                DataBase::_Self=new DataBase(DataBase::dataBaseName);
            }
        }
        else
        {
            DataBase::dataBaseName=qr;
            DataBase::_Self=new DataBase(DataBase::dataBaseName);
        }
    }
    else
    {
        if(qr==nullptr)
        {
            return DataBase::_Self;
        }
        else
        {
            DataBase::release();
            DataBase::dataBaseName=qr;
            DataBase::_Self=new DataBase(DataBase::dataBaseName);
        }

    }
    return DataBase::_Self;
}

bool DataBase::release()
{
    if(DataBase::_Self==nullptr)
    {
        return false;
    }
    delete DataBase::_Self;
    DataBase::_Self=nullptr;
    return true;

}

void DataBase::setSavePath(QString qr)
{
    DataBase::qInfoDirPath=qr;
}
///
/// \brief DataBase::initInfo
/// \param 第一个题库名称，第二个作者信息，第三个IP地址，第四个端口信息，第五个相关介绍(可选)
/// \param 默认50就行
/// \return
///
bool DataBase::initInfo(QList<QString> &data,quint32 oneCount)
{
    if(data.isEmpty()||data.size()<4) return false;
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);

    if(!query.exec("SELECT count(*) FROM tInfo")) return false;
    query.next();
    if(query.value(0).toUInt()!=0){
        query.exec("DELETE FROM tInfo");

    }
    query.prepare("INSERT INTO tInfo (title,info) VALUES (?,?)");
    for(quint32 i=0;i<4;i++)
    {
        if(i==0)
        {
            query.addBindValue("题库名称");
        }
        else if(i==1)
        {
            query.addBindValue("作者信息");
        }else if(i==2)
        {
            query.addBindValue("IP地址");
        }else
        {
            query.addBindValue("端口信息");
        }
        query.addBindValue(data[i]);

        query.exec();
    }
    if(data.size()>4)
    {
        query.addBindValue("相关介绍");
        query.addBindValue(data[4]);
    }
    else query.addBindValue(QVariant::String);
    query.exec();
    query.addBindValue("当前位置");
    query.addBindValue("0");
    query.exec();
    query.addBindValue("累计题量");
    query.addBindValue("0");
    query.exec();
    query.addBindValue("一次题量");
    query.addBindValue(oneCount);
    query.exec();
    m_DataBase.commit();
    return true;
}

bool DataBase::updateInfo(quint32 id, QString info)
{
    QSqlQuery query(m_DataBase);
    if(!query.exec(QString("UPDATE tInfo SET info='%1' WHERE id=%2").arg(info).arg(id))) return false;
    else return true;
}

QString DataBase::selectInfo(quint32 id)
{
    QSqlQuery query(m_DataBase);
    query.exec(QString("SELECT info FROM tInfo WHERE id=%1").arg(id));

    if(query.next())
    {
        return query.value(0).toString();

    }
    else
    {
        return nullptr;
    }
}


quint32 DataBase::selectIndexFromUInt(InfoType info)
{
    return selectInfo(info).toUInt();
}

void DataBase::updateIndexFromUInt(quint32 newUInt, InfoType info)
{
    updateInfo(info,QString::number(newUInt));
}


bool DataBase::insertData(QList<ProblemData>& data)
{

    //将数据插入到数据库
    if (data.isEmpty())  {
        return false;
    }

    QList<SelectProblemData> select;
    select.resize(data.size());
    select.clear();
    for(quint32 size=0;size<data.size();size++)
    {
        QList<QString> qsdata;
        qsdata.resize(data[size].option.size());
        for(quint32 optionIndex=0;optionIndex<qsdata.size();optionIndex++)
        {
            qsdata[optionIndex]=data[size].option[optionIndex].simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").remove("?").remove("？").toUpper();
        }
        select.append({data[size].topic.simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").remove("?").remove("？").toUpper(),qsdata});
    }
    m_DataBase.transaction();

    QSqlQuery query(m_DataBase);

    query.prepare("INSERT INTO tProblem (topic, A, B,C,D,reason,answer) "
                  "VALUES (?, ?, ?,?,?,?,?);");

    for(quint32 size=0;size<data.size();size++)
    {
        query.addBindValue(data[size].topic);
        quint32 miniIndex=0;
        for(;miniIndex<data[size].option.size();miniIndex++)
        {
            query.addBindValue(data[size].option[miniIndex]);
        }
        for(;miniIndex<4;miniIndex++)
        {
            query.addBindValue(QVariant::String);
        }
        if(data[size].reason.isEmpty())
        {
            query.addBindValue(QVariant::String);
        }
        else
        {
            query.addBindValue(data[size].reason);
        }

        query.addBindValue(data[size].answer);
        query.exec();
    }

    query.prepare("INSERT INTO tProblemSelect (topic,A,B,C,D) VALUES (?,?,?,?,?);");
    for(quint32 size=0;size<select.size();size++)
    {
        query.addBindValue(select[size].topic);
        quint32 miniIndex=0;
        for(;miniIndex<select[size].option.size();miniIndex++)
            query.addBindValue(select[size].option[miniIndex]);
        for(;miniIndex<4;miniIndex++)
        {
            query.addBindValue(QVariant::String);
        }
        query.exec();
    }
    m_DataBase.commit();
    return true;
}

bool DataBase::insertData(ProblemData &data)
{
    //将数据插入到数据库


    QList<QString> qsdata;
    qsdata.resize(data.option.size());
    for(quint32 optionIndex=0;optionIndex<qsdata.size();optionIndex++)
    {
        qsdata[optionIndex]=data.option[optionIndex].simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").remove("?").remove("？").toUpper();
    }
    SelectProblemData select({data.topic.simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").remove("?").remove("？").toUpper(),qsdata});

    m_DataBase.transaction();

    QSqlQuery query(m_DataBase);

    query.prepare("INSERT INTO tProblem (topic, A, B,C,D,reason,answer) "
                  "VALUES (?, ?, ?,?,?,?,?)");
    query.addBindValue(data.topic);
    quint32 miniIndex=0;
    for(;miniIndex<data.option.size();miniIndex++)
    {
        query.addBindValue(data.option[miniIndex]);
    }
    for(;miniIndex<4;miniIndex++)
    {
        query.addBindValue(QVariant::String);
    }
    if(data.reason.isEmpty())
    {
        query.addBindValue(QVariant::String);
    }
    else
    {
        query.addBindValue(data.reason);
    }

    query.addBindValue(data.answer);
    query.exec();

    query.prepare("INSERT INTO tProblemSelect (topic,A,B,C,D) VALUES (?,?,?,?,?)");

    query.addBindValue(select.topic);
    miniIndex=0;
    for(;miniIndex<select.option.size();miniIndex++)
        query.addBindValue(select.option[miniIndex]);
    for(;miniIndex<4;miniIndex++)
    {
        query.addBindValue(QVariant::String);
    }
    query.exec();
    m_DataBase.commit();
    return true;
}

bool DataBase::deleteData(QList<quint32>& index)
{
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);

    for(quint32 size=0;size<index.size();size++)
    {
        query.prepare(QString("DELETE FROM tProblem WHERE id=%1").arg(index[size]));
        query.exec();
    }
    m_DataBase.commit();

    return true;
}

bool DataBase::deleteData(quint32 index)
{
    QSqlQuery query(m_DataBase);
    query.prepare(QString("DELETE FROM tProblem WHERE id=%1").arg(index));
    return query.exec();
}

bool DataBase::deleteAllData()
{
    QSqlQuery query(m_DataBase);
    query.prepare("DELETE FROM tProblem");
    return query.exec();
}

bool DataBase::updateData(QList<ProblemData> &data, QList<quint32> &index)
{
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    for(quint32 size=0;size<index.size();size++)
    {
        if(data[size].option.size()==2)
        {
            query.prepare(QString("UPDATE tProblem SET topic = '%1',A= '%2',B='%3',reason='%4',answer=%5 WHERE id =%6").arg(data[size].topic).arg(data[size].option[0]).
                    arg(data[size].option[1]).arg(data[size].reason).arg(data[size].answer).arg(index[size]));
        }
        else if(data[size].option.size()==3)
        {
            query.prepare(QString("UPDATE tProblem SET topic = '%1',A= '%2',B='%3',C='%4',reason='%5',answer=%6 WHERE id =%7").arg(data[size].topic).arg(data[size].option[0]).
                    arg(data[size].option[1]).arg(data[size].option[2]).arg(data[size].reason).arg(data[size].answer).arg(index[size]));
        }
        else
        {

            query.prepare(QString("UPDATE tProblem SET topic = '%1',A= '%2',B='%3',C='%4',D='%5',reason='%6',answer=%7 WHERE id =%8").arg(data[size].topic).arg(data[size].option[0]).
                    arg(data[size].option[1]).arg(data[size].option[2]).arg(data[size].option[3]).arg(data[size].reason).arg(data[size].answer).arg(index[size]));
        }
        query.exec();
    }
    m_DataBase.commit();
    return true;
}

bool DataBase::updateData(ProblemData &data, quint32 index)
{
    QList<QString> qsdata;
    qsdata.resize(data.option.size());
    for(quint32 optionIndex=0;optionIndex<qsdata.size();optionIndex++)
    {
        qsdata[optionIndex]=data.option[optionIndex].simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").toUpper();
    }
    SelectProblemData select({data.topic.simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").toUpper(),qsdata});
    QSqlQuery query(m_DataBase);
    if(data.option.size()==2)
    {
        query.prepare(QString("UPDATE tProblem SET topic = '%1',A= '%2',B='%3',reason='%4',answer=%5 WHERE id =%6").arg(data.topic).arg(data.option[0]).
                arg(data.option[1]).arg(data.reason).arg(data.answer).arg(index));
        query.exec();
        query.prepare(QString("UPDATE tProblemSelect SET topic = '%1',A= '%2',B='%3' WHERE id =%4").arg(data.topic).arg(data.option[0]).
                arg(data.option[1]).arg(index));
        return query.exec();
    }
    else if(data.option.size()==3)
    {
        query.prepare(QString("UPDATE tProblem SET topic = '%1',A= '%2',B='%3',C='%4',reason='%5',answer=%6 WHERE id =%7").arg(data.topic).arg(data.option[0]).
                arg(data.option[1]).arg(data.option[2]).arg(data.reason).arg(data.answer).arg(index));
        query.exec();
        query.prepare(QString("UPDATE tProblemSelect SET topic = '%1',A= '%2',B='%3',C='%4' WHERE id =%5").arg(data.topic).arg(data.option[0]).
                arg(data.option[1]).arg(data.option[2]).arg(index));
        return query.exec();
    }
    else if(data.option.size()==4)
    {

        query.prepare(QString("UPDATE tProblem SET topic = '%1',A= '%2',B='%3',C='%4',D='%5',reason='%6',answer=%7 WHERE id =%8").arg(data.topic).arg(data.option[0]).
                arg(data.option[1]).arg(data.option[2]).arg(data.option[3]).arg(data.reason).arg(data.answer).arg(index));
        query.exec();
        query.prepare(QString("UPDATE tProblemSelect SET topic = '%1',A= '%2',B='%3',C='%4',D='%5' WHERE id =%6").arg(data.topic).arg(data.option[0]).
                arg(data.option[1]).arg(data.option[2]).arg(data.option[3]).arg(index));
        return query.exec();
    }
    return false;
}

bool DataBase::searchData(QList<QString>& info,QList<ProblemData>& problem,QList<quint32>& index, SearchWay way)
{
    bool success=false;
    if(info.isEmpty())
    {
        return success;
    }
    m_DataBase.transaction();
    for(quint32 index=0;index<info.size();index++)
    {
        info[index]=info[index].simplified().remove(QChar('"')).remove(QString("“")).remove(QString("”")).remove(".").remove("。").remove(",").remove("，").remove("'").remove("‘").remove("’").remove("`").remove("(").remove(")").remove("（").remove("）").remove("、").toUpper();
    }
    QSqlQuery query(m_DataBase);
    QString selectString;
    if(way==SearchWay::FromTopic)
    {
        selectString=QString("SELECT * FROM tProblem INNER JOIN tProblemSelect ON tProblemSelect.id=tProblem.id WHERE tProblemSelect.topic LIKE '%%1%'").arg(info[0]);
        for(quint32 miniIndex=1;miniIndex<info.size();miniIndex++)
        {
            selectString+=QString("AND tProblemSelect.topic LIKE '%%1%'").arg(info[miniIndex]);
        }

    }
    else
    {
        selectString=QString("SELECT * FROM tProblem INNER JOIN tProblemSelect ON tProblemSelect.id=tProblem.id WHERE (tProblemSelect.A LIKE '%%1%' OR tProblemSelect.B LIKE '%%1%' OR tProblemSelect.C LIKE '%%1%' OR tProblemSelect.D LIKE '%%1%')").arg(info[0]);
        for(quint32 miniIndex=1;miniIndex<info.size();miniIndex++)
        {
            selectString+=QString("AND (tProblemSelect.A LIKE '%%1%' OR tProblemSelect.B LIKE '%%1%' OR tProblemSelect.C LIKE '%%1%' OR tProblemSelect.D LIKE '%%1%')").arg(info[miniIndex]);
        }
    }
    selectString+=";";
    success=query.exec(selectString);

    if(!success)
    {
        return success;
    }
    problem.clear();
    index.clear();
    while(query.next())
    {
        ProblemData pro;
        index.append(query.value(0).toUInt());
        pro.topic=query.value(1).toString();
        pro.option.clear();
        pro.option.append(query.value(2).toString());
        pro.option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            pro.option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                pro.option.append(query.value(5).toString());
            }
        }
        if(!query.value(6).isNull())
        {
            pro.reason=query.value(6).toString();
        }else
        {
            pro.reason.clear();
        }
        pro.answer=query.value(7).toUInt();
        problem.append(pro);
    }
    m_DataBase.commit();
    return success;
}

bool DataBase::getData(QList<ProblemData> &data, QList<quint32> &index,quint32 count)
{
    bool success=false;
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    data.clear();
    index.clear();
    success=query.exec(QString("SELECT * FROM tProblem ORDER BY RANDOM() limit %1").arg(count));
    if(!success) return false;
    while(query.next())
    {
        ProblemData pro;
        index.append(query.value(0).toUInt());
        pro.topic=query.value(1).toString();
        pro.option.clear();
        pro.option.append(query.value(2).toString());
        pro.option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            pro.option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                pro.option.append(query.value(5).toString());
            }
        }
        if(!query.value(6).isNull())
        {
            pro.reason=query.value(6).toString();
        }
        else
        {
            pro.reason.clear();
        }
        pro.answer=query.value(7).toUInt();
        data.append(pro);
    }
    m_DataBase.commit();
    return success;


}

bool DataBase::getData(QList<ProblemData> &data, QList<quint32> &index, quint32 offset, quint32 count)
{
    bool success=false;

    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    data.clear();
    index.clear();

    success=query.exec(QString("SELECT * FROM tProblem ORDER BY id limit %1 offset %2").arg(count).arg(offset));

    if(!success) return false;
    while(query.next())
    {
        ProblemData pro;
        index.append(query.value(0).toUInt());
        pro.topic=query.value(1).toString();
        pro.option.clear();
        pro.option.append(query.value(2).toString());
        pro.option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            pro.option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                pro.option.append(query.value(5).toString());
            }
        }
        if(!query.value(6).isNull())
        {
            pro.reason=query.value(6).toString();
        }else
        {
            pro.reason.clear();
        }
        pro.answer=query.value(7).toUInt();
        data.append(pro);
    }
    m_DataBase.commit();
    return success;


}

bool DataBase::getLastData(QList<ProblemData> &data, QList<quint32> &index)
{
    bool success=false;

    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);

    success=query.exec("select * from tProblem order by id desc limit 1;");
    if(!success) return false;
    if(query.next())
    {
        ProblemData pro;
        index.append(query.value(0).toUInt());
        pro.topic=query.value(1).toString();
        pro.option.clear();
        pro.option.append(query.value(2).toString());
        pro.option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            pro.option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                pro.option.append(query.value(5).toString());
            }
        }
        if(!query.value(6).isNull())
        {
            pro.reason=query.value(6).toString();
        }else
        {
            pro.reason.clear();
        }
        pro.answer=query.value(7).toUInt();
        data.append(pro);
    }
    m_DataBase.commit();
    return success;
}

bool DataBase::createNewDataFromFile(QString fileName,QList<QString> &path, QList<QString> &info, QString splitTopic, QString splitOneTopic)
{

    if(path.empty()||info.empty()) return false;


    /*第二步，载入题目文件和答案文件*/
    QList<ProblemData> bigProblem;
    bigProblem.clear();
    DataBase::release();
    for(auto one:path)
    {
        QList<ProblemData> problem;
        problem.clear();
        QFileInfo fileinfo(one);
        if(fileinfo.suffix().toUpper()=="TXT")
        {
            QFile oneFile(one);

            //打开失败后的处理
            if(!oneFile.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                continue;
            }
            int indexget[8]={0};
            int typeClass=4;
            QTextStream in(&oneFile);
            if(!in.atEnd())
            {
                QString line=in.readLine();
                QStringList titleList=line.split(splitOneTopic,Qt::SkipEmptyParts);
                indexget[0]=titleList.indexOf("题目");
                if(indexget[0]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[1]=titleList.indexOf("A");
                if(indexget[1]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[2]=titleList.indexOf("B");
                if(indexget[2]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[3]=titleList.indexOf("C");
                if(indexget[3]!=-1)
                {
                    typeClass++;
                }
                indexget[4]=titleList.indexOf("D");
                if(indexget[4]!=-1)
                {
                    typeClass++;
                }
                indexget[5]=titleList.indexOf("答案");
                if(indexget[5]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[6]=titleList.indexOf("解析");
                if(indexget[6]!=-1)
                {
                    typeClass++;
                }
                indexget[7]=titleList.indexOf("多选");
                if(indexget[7]!=-1)
                {
                    typeClass++;
                }
            }
            while(!in.atEnd())
            {
                QString line=in.readLine();
                line=line.trimmed();
                line.replace(splitTopic,"\n");
                QStringList typeArray=line.split(splitOneTopic,Qt::SkipEmptyParts);
                if(typeArray.size()!=typeClass) continue;
                ProblemData oneProblem;
                oneProblem.topic=typeArray[indexget[0]];
                oneProblem.option.clear();
                for(quint32 q=1;q<5;q++)
                {
                    if(indexget[q]!=-1)
                        oneProblem.option.append(typeArray[indexget[q]]);
                }
                oneProblem.answer=0;
                if(typeArray[indexget[5]].contains('A'))
                {
                    oneProblem.answer=oneProblem.answer|1;
                }
                if(typeArray[indexget[5]].contains('B'))
                {
                    oneProblem.answer=oneProblem.answer|2;
                }
                if(typeArray[indexget[5]].contains('C'))
                {
                    oneProblem.answer=oneProblem.answer|4;
                }
                if(typeArray[indexget[5]].contains('D'))
                {
                    oneProblem.answer=oneProblem.answer|8;
                }
                if(indexget[6]!=-1)
                    oneProblem.reason=typeArray[indexget[6]];
                else oneProblem.reason.clear();
                if(indexget[7]!=-1&&typeArray[indexget[7]]=="是")
                {
                    oneProblem.answer=oneProblem.answer|16;
                }
                problem.append(oneProblem);

            }
            oneFile.close();
        }
        else if(fileinfo.suffix().toUpper()=="XLS"||fileinfo.suffix().toUpper()=="XLSX")
        {

            /*注意第一行是字段：题目,A,B,C,D,答案,解析,多选*/
            QAxObject *excel=new QAxObject("Excel.Application");
            if(!excel) continue;
            QAxObject *workbooks = excel->querySubObject("WorkBooks");
            QAxObject *workbook=workbooks->querySubObject("Open(QString, QVariant)", one);
            QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);
            QAxObject * usedrange = worksheet->querySubObject("UsedRange");
            QAxObject * rows = usedrange->querySubObject("Rows");
            int intRows = rows->property("Count").toInt();
            QString Range = "A1:H" +QString::number(intRows);
            QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
            QVariant allEnvDataQVariant = allEnvData->property("Value");
            QVariantList allEnvDataList = allEnvDataQVariant.toList();
            int indexget[8]={0};
            QVariantList titleList=allEnvDataList[0].toList();
            indexget[0]=titleList.indexOf<QString>("题目");
            if(indexget[0]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[1]=titleList.indexOf<QString>("A");
            if(indexget[1]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[2]=titleList.indexOf<QString>("B");
            if(indexget[2]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[3]=titleList.indexOf<QString>("C");
            indexget[4]=titleList.indexOf<QString>("D");
            indexget[5]=titleList.indexOf<QString>("答案");
            if(indexget[5]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[6]=titleList.indexOf<QString>("解析");
            indexget[7]=titleList.indexOf<QString>("多选");
            for(int i=1; i< intRows; i++)
            {
                ProblemData oneProblem;
                QVariantList  oneList =  allEnvDataList[i].toList() ;
                oneProblem.topic=oneList[indexget[0]].toString();
                oneProblem.option.clear();
                oneProblem.answer=0;
                for(quint32 start=1;start<5;start++)
                {
                    if(indexget[start]!=-1)
                        oneProblem.option.append(oneList[indexget[start]].toString());
                }
                QString inforMation=oneList[indexget[5]].toString();
                if(inforMation.contains('A'))
                {
                    oneProblem.answer=oneProblem.answer|1;
                }
                if(inforMation.contains('B'))
                {
                    oneProblem.answer=oneProblem.answer|2;
                }
                if(inforMation.contains('C'))
                {
                    oneProblem.answer=oneProblem.answer|4;
                }
                if(inforMation.contains('D'))
                {
                    oneProblem.answer=oneProblem.answer|8;
                }
                if(indexget[6]!=-1)
                {
                    oneProblem.reason=oneList[indexget[6]].toString();
                }
                else oneProblem.reason.clear();
                if(indexget[7]!=-1&&oneList[indexget[7]].toString()=="是")
                {
                    oneProblem.answer=oneProblem.answer|16;
                }
                problem.append(oneProblem);
            }
            workbooks->dynamicCall("Close()");
            excel->dynamicCall("Quit(void)");
            delete excel;

        }
        else if(fileinfo.suffix().toUpper()=="DB")
        {
            DataBase datas(fileinfo.baseName());
            datas.exportData(problem);
        }
        bigProblem.append(problem);
    }
    DataBase data(fileName);
    data.initInfo(info);
    data.insertData(bigProblem);
    return true;
}

bool DataBase::loadDataFromFile(QList<QString>& path,QString splitTopic,QString splitOneTopic)
{
    if(path.empty()) return false;
    /*第二步，载入题目文件和答案文件*/
    for(auto one:path)
    {
        QList<ProblemData> problem;
        problem.clear();
        QFileInfo fileinfo(one);
        if(fileinfo.suffix().toUpper()=="TXT")
        {
            QFile oneFile(one);

            //打开失败后的处理
            if(!oneFile.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                continue;
            }
            int indexget[8]={0};
            int typeClass=4;
            QTextStream in(&oneFile);
            if(!in.atEnd())
            {
                QString line=in.readLine();
                QStringList titleList=line.split(splitOneTopic,Qt::SkipEmptyParts);
                indexget[0]=titleList.indexOf("题目");
                if(indexget[0]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[1]=titleList.indexOf("A");
                if(indexget[1]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[2]=titleList.indexOf("B");
                if(indexget[2]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[3]=titleList.indexOf("C");
                if(indexget[3]!=-1)
                {
                    typeClass++;
                }
                indexget[4]=titleList.indexOf("D");
                if(indexget[4]!=-1)
                {
                    typeClass++;
                }
                indexget[5]=titleList.indexOf("答案");
                if(indexget[5]==-1)
                {
                    oneFile.close();
                    continue;
                }
                indexget[6]=titleList.indexOf("解析");
                if(indexget[6]!=-1)
                {
                    typeClass++;
                }
                indexget[7]=titleList.indexOf("多选");
                if(indexget[7]!=-1)
                {
                    typeClass++;
                }
            }
            while(!in.atEnd())
            {
                QString line=in.readLine();
                line=line.trimmed();
                line.replace(splitTopic,"\n");
                QStringList typeArray=line.split(splitOneTopic,Qt::SkipEmptyParts);
                if(typeArray.size()!=typeClass) continue;
                ProblemData oneProblem;
                oneProblem.topic=typeArray[indexget[0]];
                oneProblem.option.clear();
                for(quint32 q=1;q<5;q++)
                {
                    if(indexget[q]!=-1)
                        oneProblem.option.append(typeArray[indexget[q]]);
                }

                oneProblem.answer=0;
                if(typeArray[indexget[5]].contains('A'))
                {
                    oneProblem.answer=oneProblem.answer|1;
                }
                if(typeArray[indexget[5]].contains('B'))
                {
                    oneProblem.answer=oneProblem.answer|2;
                }
                if(typeArray[indexget[5]].contains('C'))
                {
                    oneProblem.answer=oneProblem.answer|4;
                }
                if(typeArray[indexget[5]].contains('D'))
                {
                    oneProblem.answer=oneProblem.answer|8;
                }
                if(indexget[6]!=-1)
                    oneProblem.reason=typeArray[indexget[6]];
                else oneProblem.reason.clear();
                if(indexget[7]!=-1&&typeArray[indexget[7]]=="是")
                {
                    oneProblem.answer=oneProblem.answer|16;
                }
                problem.append(oneProblem);

            }
            oneFile.close();
        }
        else if(fileinfo.suffix().toUpper()=="XLS"||fileinfo.suffix().toUpper()=="XLSX")
        {
            /*注意第一行是字段：题目,A,B,C,D,答案,解析,多选*/
            QAxObject *excel=new QAxObject("Excel.Application");
            if(!excel) continue;
            QAxObject *workbooks = excel->querySubObject("WorkBooks");
            QAxObject *workbook=workbooks->querySubObject("Open(QString, QVariant)", one);
            QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);
            QAxObject * usedrange = worksheet->querySubObject("UsedRange");
            QAxObject * rows = usedrange->querySubObject("Rows");
            int intRows = rows->property("Count").toInt();
            QString Range = "A1:H" +QString::number(intRows);
            QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
            QVariant allEnvDataQVariant = allEnvData->property("Value");
            QVariantList allEnvDataList = allEnvDataQVariant.toList();
            int indexget[8]={0};
            QVariantList titleList=allEnvDataList[0].toList();
            indexget[0]=titleList.indexOf<QString>("题目");
            if(indexget[0]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[1]=titleList.indexOf<QString>("A");
            if(indexget[1]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[2]=titleList.indexOf<QString>("B");
            if(indexget[2]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[3]=titleList.indexOf<QString>("C");
            indexget[4]=titleList.indexOf<QString>("D");
            indexget[5]=titleList.indexOf<QString>("答案");
            if(indexget[5]==-1)
            {
                workbooks->dynamicCall("Close()");
                excel->dynamicCall("Quit(void)");
                delete excel;
                continue;
            }
            indexget[6]=titleList.indexOf<QString>("解析");
            indexget[7]=titleList.indexOf<QString>("多选");
            for(int i=1; i< intRows; i++)
            {
                ProblemData oneProblem;
                oneProblem.answer=0;
                QVariantList  oneList =  allEnvDataList[i].toList() ;
                oneProblem.topic=oneList[indexget[0]].toString();
                oneProblem.option.clear();
                for(quint32 start=1;start<5;start++)
                {
                    if(indexget[start]!=-1)
                        oneProblem.option.append(oneList[indexget[start]].toString());
                }
                QString inforMation=oneList[indexget[5]].toString();
                if(inforMation.contains('A'))
                {
                    oneProblem.answer=oneProblem.answer|1;
                }
                if(inforMation.contains('B'))
                {
                    oneProblem.answer=oneProblem.answer|2;
                }
                if(inforMation.contains('C'))
                {
                    oneProblem.answer=oneProblem.answer|4;
                }
                if(inforMation.contains('D'))
                {
                    oneProblem.answer=oneProblem.answer|8;
                }
                if(indexget[6]!=-1)
                {
                    oneProblem.reason=oneList[indexget[6]].toString();
                }
                else oneProblem.reason.clear();
                if(indexget[7]!=-1&&oneList[indexget[7]].toString()=="是")
                {
                    oneProblem.answer=oneProblem.answer|16;
                }
                problem.append(oneProblem);

            }
            workbooks->dynamicCall("Close()");
            excel->dynamicCall("Quit(void)");
            delete excel;
        }
        else if(fileinfo.suffix().toUpper()=="DB")
        {
            DataBase datas(fileinfo.baseName());
            datas.exportData(problem);
        }
        this->get();
        this->insertData(problem);
    }
    return true;
}

bool DataBase::exportData(QList<ProblemData> &data)
{
    QSqlQuery query(m_DataBase);
    if(!query.exec("SELECT * from tProblem")) return false;
    while(query.next())
    {
        ProblemData pro;
        pro.topic=query.value(1).toString();
        pro.option.clear();
        pro.option.append(query.value(2).toString());
        pro.option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            pro.option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                pro.option.append(query.value(5).toString());
            }
        }
        if(!query.value(6).isNull())
        {
            pro.reason=query.value(6).toString();
        }
        pro.answer=query.value(7).toUInt();
        data.append(pro);
    }
    return true;
}

bool DataBase::getDataFromError(QList<ProblemData> &data, QList<quint32> &index, quint32 count)
{
    bool success=false;
    QSqlQuery query(m_DataBase);
    data.clear();
    index.clear();
    success=query.exec(QString("SELECT * FROM tProblem INNER JOIN tProblemError ON tProblemError.id=tProblem.id ORDER BY RANDOM() limit %1").arg(count));
    if(!success) return false;
    while(query.next())
    {
        ProblemData pro;
        index.append(query.value(0).toUInt());
        pro.topic=query.value(1).toString();
        pro.option.clear();
        pro.option.append(query.value(2).toString());
        pro.option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            pro.option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                pro.option.append(query.value(5).toString());
            }
        }
        if(!query.value(6).isNull())
        {
            pro.reason=query.value(6).toString();
        }
        else
        {
            pro.reason.clear();
        }
        pro.answer=query.value(7).toUInt();
        data.append(pro);
    }
    return success;
}

bool DataBase::updateDataFromError(QList<quint32> &index,QList<bool>& isOk)
{
    if(index.empty()||isOk.empty())
    {
        return false;
    }
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    for(quint32 size=0;size<index.size();size++)
    {
        query.exec(QString("SELECT id FROM tProblemError WHERE id= %1 limit 1").arg(index[size]));
        if(query.next())
        {
            if(isOk[size]==true)
            {
                query.prepare(QString("UPDATE tProblemError SET errorCount=errorCount-1 WHERE id= %1").arg(index[size]));
            }
            else
            {
                //这里设置错题记录次数
                query.prepare(QString("UPDATE tProblemError SET errorCount=4 WHERE id =%1").arg(index[size]));
            }

        }
        else
        {
            query.prepare(QString("INSERT INTO tProblemError VALUES(%1,4)").arg(index[size]));
        }
        query.exec();
    }
    query.exec("DELETE FROM tProblemError WHERE errorCount=0");
    m_DataBase.commit();
    return true;
}

bool DataBase::getDataFromRecord(QList<ProblemData> &data, QList<quint32> &index, quint32 count)
{
    bool success=false;
    QSqlQuery query(m_DataBase);
    data.clear();
    index.clear();
    success=query.exec(QString("SELECT * FROM tProblem INNER JOIN tProblemRecord ON tProblemRecord.id=tProblem.id ORDER BY RANDOM() limit %1").arg(count));
    if(!success) return false;
    while(query.next())
    {
        ProblemData pro;
        index.append(query.value(0).toUInt());
        pro.topic=query.value(1).toString();
        pro.option.clear();
        pro.option.append(query.value(2).toString());
        pro.option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            pro.option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                pro.option.append(query.value(5).toString());
            }
        }
        if(!query.value(6).isNull())
        {
            pro.reason=query.value(6).toString();
        }
        else
        {
            pro.reason.clear();
        }
        pro.answer=query.value(7).toUInt();
        data.append(pro);

    }
    return success;
}

bool DataBase::updateDataFromRecord(QList<quint32> &index)
{
    if(index.empty()) return false;
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    for(quint32 size=0;size<index.size();size++)
    {
        query.exec(QString("SELECT id FROM tProblemRecord WHERE id= %1 limit 1").arg(index[size]));
        if(query.next())
        {
            query.prepare(QString("DELETE FROM tProblemRecord WHERE id= %1").arg(index[size]));
        }
        else
        {
            //记录
            query.prepare(QString("INSERT INTO tProblemRecord VALUES(%1)").arg(index[size]));
        }
        query.exec();
    }
    m_DataBase.commit();
    return true;
}

bool DataBase::updateDataFromRecord(quint32 index)
{
    QSqlQuery query(m_DataBase);
    query.exec(QString("SELECT id FROM tProblemRecord WHERE id= %1 limit 1").arg(index));
    if(query.next())
    {
        query.prepare(QString("DELETE FROM tProblemRecord WHERE id= %1").arg(index));
    }
    else
    {
        //记录
        query.prepare(QString("INSERT INTO tProblemRecord VALUES(%1)").arg(index));
    }
    return query.exec();
}

bool DataBase::panDataFromRecord(QList<quint32> &index, QList<bool> &isIn)
{
    if(index.empty()||isIn.empty()) return false;
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    for(quint32 size=0;size<index.size();size++)
    {
        query.exec(QString("SELECT id FROM tProblemRecord WHERE id= %1 limit 1").arg(index[size]));
        if(query.next())
        {
            isIn[size]=true;
        }
        else
        {
            isIn[size]=false;
        }
    }
    m_DataBase.commit();
    return true;
}

bool DataBase::panDataFromError(QList<quint32> &index, QList<bool> &isIn)
{
    if(index.empty()||isIn.empty()) return false;
    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    for(quint32 size=0;size<index.size();size++)
    {
        query.exec(QString("SELECT id FROM tProblemError WHERE id= %1 limit 1").arg(index[size]));
        if(query.next())
        {
            isIn[size]=true;
        }
        else
        {
            isIn[size]=false;
        }
    }
    m_DataBase.commit();
    return true;
}

bool DataBase::getDataCount(quint32 *problemCount, quint32 *errorProblemCount, quint32 *recordProblemCount)
{
    int success=3;
    QSqlQuery query(m_DataBase);
    if(problemCount!=nullptr)
    {
        success-=query.exec("SELECT count(*) FROM tProblemSelect")==false;
        query.next();
        *problemCount=query.value(0).toUInt();
    }
    if(errorProblemCount!=nullptr)
    {
        success-=query.exec("SELECT count(*) FROM tProblemError")==false;
        query.next();
        *errorProblemCount=query.value(0).toUInt();
    }
    if(recordProblemCount!=nullptr)
    {
        success-=query.exec("SELECT count(*) FROM tProblemRecord")==false;
        query.next();
        *recordProblemCount=query.value(0).toUInt();
    }
    return success==3;
}

bool DataBase::deleteData()
{
    QList<QString> topicArray;
    QList<QVector<QString>> optionArray;
    QList<quint8> answerArray;
    QList<quint32> idArray;
    QList<quint32> cannotPanArray;
    topicArray.clear();
    optionArray.clear();
    answerArray.clear();
    idArray.clear();
    cannotPanArray.clear();
    bool success=false;

    m_DataBase.transaction();
    QSqlQuery query(m_DataBase);
    success=query.exec(QString("SELECT tProblemSelect.id,tProblemSelect.topic,tProblemSelect.A,tProblemSelect.B,tProblemSelect.C,tProblemSelect.D,tProblem.answer FROM tProblemSelect INNER JOIN tProblem ON tProblemSelect.id=tProblem.id;"));

    if(!success) return false;
    while(query.next())
    {
        idArray.append(query.value(0).toUInt());
        topicArray.append(query.value(1).toString());

        QVector<QString> option;
        option.clear();
        option.append(query.value(2).toString());
        option.append(query.value(3).toString());
        if(!query.value(4).isNull())
        {
            option.append(query.value(4).toString());
            if(!query.value(5).isNull())
            {
                option.append(query.value(5).toString());
            }
        }
        std::sort(option.begin(),option.end());

        optionArray.append(option);
        answerArray.append(query.value(6).toUInt());

    }
    m_DataBase.commit();

    quint32 i,j,mini;
    int pan;

    for(i=0;i<idArray.size();i++)
    {
        for(j=i+1;j<idArray.size();j++)
        {

            pan=0;
            //包含关系取最长的
            if(topicArray[i].contains(topicArray[j]))
            {
                pan=1;
            }
            else if(topicArray[j].contains(topicArray[i]))
            {
                pan=2;
            }

            if(!pan||optionArray[i].size()!=optionArray[j].size())  continue;
            for(mini=0;mini<optionArray[i].size();mini++)
            {
                if(!(optionArray[i][mini].contains(optionArray[j][mini])||optionArray[j][mini].contains(optionArray[i][mini])))
                {
                    pan=0;
                    break;
                }
            }
            if(pan!=0)
            {
                if(answerArray[i]==answerArray[j])
                {

                    if(pan==1)
                    {
                        idArray.removeAt(j);
                        optionArray.remove(j);
                        answerArray.remove(j);
                        topicArray.remove(j);
                    }
                    else
                    {
                        idArray.removeAt(i);
                        optionArray.remove(i);
                        answerArray.remove(i);
                        topicArray.remove(i);
                        i--;
                    }
                }
                else
                {

                    cannotPanArray.append(idArray[i]);
                    cannotPanArray.append(idArray[j]);
                    optionArray.remove(i);
                    answerArray.remove(i);
                    topicArray.remove(i);
                    optionArray.remove(j);
                    answerArray.remove(j);
                    topicArray.remove(j);
                    idArray.removeAt(i);
                    idArray.removeAt(j-1);
                    i--;
                }
                break;
            }
        }
    }

    m_DataBase.transaction();
    topicArray.clear();
    for(auto oos:optionArray)
    {
        oos.clear();
    }
    optionArray.clear();
    answerArray.clear();
    QList<ProblemData> bigProblemData;
    QList<ProblemData> cannotPanProblem;
    m_DataBase.transaction();
    for(quint32 size=0;size<idArray.size();size++)
    {
        query.exec(QString("SELECT topic,A,B,C,D,reason,answer FROM tProblem WHERE id= %1 limit 1").arg(idArray[size]));
        if(query.next())
        {
            ProblemData pro;
            pro.topic=query.value(0).toString();
            pro.option.clear();
            pro.option.append(query.value(1).toString());
            pro.option.append(query.value(2).toString());
            if(!query.value(3).isNull())
            {
                pro.option.append(query.value(3).toString());
                if(!query.value(4).isNull())
                {
                    pro.option.append(query.value(4).toString());
                }
            }
            if(!query.value(5).isNull())
            {
                pro.reason=query.value(5).toString();
            }
            else
            {
                pro.reason.clear();
            }
            pro.answer=query.value(6).toUInt();
            bigProblemData.append(pro);
        }
    }
    m_DataBase.commit();
    m_DataBase.transaction();

    for(quint32 size=0;size<cannotPanArray.size();size++)
    {
        query.exec(QString("SELECT topic,A,B,C,D,reason,answer FROM tProblem WHERE id= %1 limit 1").arg(cannotPanArray[size]));
        if(query.next())
        {
            ProblemData pro;
            pro.topic=query.value(0).toString();
            pro.option.clear();
            pro.option.append(query.value(1).toString());
            pro.option.append(query.value(2).toString());
            if(!query.value(3).isNull())
            {
                pro.option.append(query.value(3).toString());
                if(!query.value(4).isNull())
                {
                    pro.option.append(query.value(4).toString());
                }
            }
            if(!query.value(5).isNull())
            {
                pro.reason=query.value(5).toString();
            }
            else
            {
                pro.reason.clear();
            }
            pro.answer=query.value(6).toUInt();
            cannotPanProblem.append(pro);
        }

    }
    m_DataBase.commit();
    QFileInfo file;
    if(file.exists(qInfoDirPath+"!!!RemoveDuplicates.db"))
    {
        DataBase* data=DataBase::get("!!!RemoveDuplicates");
        data->insertData(bigProblemData);

    }
    else
    {
        /*
        for(int ik=1;ik<=5;ik++)
        {
            infoList.append(selectInfo(ik));
        }
        oneCount=selectIndexFromUInt(InfoType::OneCount);
        */
        DataBase::release();
        DataBase datas("!!!RemoveDuplicates");
        auto loadInfo=QList<QString>({"处理记载","IllusionAutumn","127.0.0.1","9427","这个文件里是去除重复过后的题目"});
        datas.initInfo(loadInfo,50);
        datas.insertData(bigProblemData);
    }
    if(cannotPanProblem.size())
    {
        if(file.exists(qInfoDirPath+"!!!ErrorTopic.db"))
        {
            DataBase* data=DataBase::get("!!!ErrorTopic");
            data->insertData(cannotPanProblem);
        }
        else
        {
            DataBase data("!!!ErrorTopic");
            auto loadInfo=QList<QString>({"错误记载","IllusionAutumn","127.0.0.1","9427","这个文件是用来记录处理过程中答案不匹配的题目的"});
            data.initInfo(loadInfo,50);
            data.insertData(cannotPanProblem);

        }

    }
    return success;
}
