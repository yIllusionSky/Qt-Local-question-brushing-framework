//此处借鉴：https://www.cnblogs.com/tingtaishou/p/11994480.html
#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include<QList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>


enum SearchWay:quint8
{
    FromTopic,FromOption
};
enum InfoType:quint8
{
    Index=6,Count=7,OneCount=8
};

/*Answer对应的1,2,4,8是ABCD;*/
struct ProblemData
{
    QString topic;
    QList<QString> option;
    quint8 answer;
    QString reason;

};

struct SelectProblemData
{
    QString topic;
    QList<QString> option;
};
class DataBase;
class DataBase : public QObject
{
    Q_OBJECT

public:
    //如果数据库存在，则返回数据库指针，否则创建一个新的数据库
    static DataBase *get(QString qr=nullptr);
    static bool release();

    static void setSavePath(QString qr);
    /*对基本数据进行操作*/
    bool initInfo(QList<QString>&data,quint32 oneCount=50);
    bool updateInfo(quint32 id,QString info);
    QString selectInfo(quint32 id);
    quint32 selectIndexFromUInt(InfoType info=InfoType::Index);
    void updateIndexFromUInt(quint32 newUInt,InfoType info=InfoType::Index);
    bool insertData(QList<ProblemData>& data);
    bool insertData(ProblemData& data);
    bool deleteData(QList<quint32>& index);
    bool deleteData(quint32 index);
    bool deleteAllData();
    bool updateData(QList<ProblemData>& data,QList<quint32>& index);
    bool updateData(ProblemData& data,quint32 index);
    bool searchData(QList<QString>& info,QList<ProblemData>& problem,QList<quint32>& index,SearchWay way=SearchWay::FromTopic);
    bool getData(QList<ProblemData>& data,QList<quint32>& index,quint32 count);
    bool getData(QList<ProblemData>& data,QList<quint32>& index,quint32 offset,quint32 count);
    bool getLastData(QList<ProblemData>& data,QList<quint32>& index);
    static bool createNewDataFromFile(QString fileName,QList<QString>& path,QList<QString>& info,QString enterReplace="\\n",QString splitOneTopic="\t");
    bool loadDataFromFile(QList<QString>& path,QString enterReplace="\\n",QString splitOneTopic="\t");
    bool exportData(QList<ProblemData>& data);
    /*数据的高级操作*/
    bool getDataFromError(QList<ProblemData>& data,QList<quint32>&index,quint32 count=50);
    bool updateDataFromError(QList<quint32>&index,QList<bool>& isOk);
    bool getDataFromRecord(QList<ProblemData>& data,QList<quint32>&index,quint32 count=50);
    bool updateDataFromRecord(QList<quint32>&index);
    bool updateDataFromRecord(quint32 index);
    bool panDataFromRecord(QList<quint32>& index,QList<bool>& isIn);
    bool panDataFromError(QList<quint32>& index,QList<bool>& isIn);
    /*获取数据个数*/
    bool getDataCount(quint32* problemCount,quint32* errorProblemCount=nullptr,quint32* recordProblemCount=nullptr);

    /*删除数据*/
    bool deleteData();
private:

    QString dataName;
    QSqlDatabase m_DataBase;
    static DataBase* _Self;         //自己
    static QString dataBaseName;    //数据库文件名
    static QString qInfoDirPath;    //默认Save文件夹位置
    //创建与取消创建
    DataBase(QString name, QObject *parent=0);
    ~DataBase();

    //相关操作
    bool initTable();
    bool uninitTable();
    bool isExistTable(QString table);
    bool createTable(QString table);
};
#endif // DATABASE_H
