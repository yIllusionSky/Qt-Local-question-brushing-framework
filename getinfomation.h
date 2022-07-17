#ifndef GETINFOMATION_H
#define GETINFOMATION_H
//程序中用到的头文件
#include <QProcess>
#include<QDebug>
QString getInfo(const QString &cmd)
{
    qDebug()<<cmd;
    QProcess p;				//启动外部程序
    p.start(cmd);			//一体式启动，不分离，主程序退出则启动程序退出,使用close关闭
    //p.startDetached(cmd)	//分离式启动，主程序退出后，外部程序继续运行。
    p.waitForFinished(-1);	//超时等待，设置为-1，直到执行完成。
    QString result = QString::fromLocal8Bit(p.readAll());

    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    p.close();
    return result;
}
//查询CPU型号
QString getCpuName()
{
    return getInfo("wmic cpu get Name");
}
//查询CPU核心数
QString getCpuCore()
{
    return getInfo("wmic cpu get NumberOfCores");
}

QString getCpuProcessors()
{
    return getInfo("wmic cpu get NumberOfLogicalProcessors");
}
//查询CPU***
QString getCpuProcessorid()
{
     return getInfo("wmic cpu get processorid");
}
//查询主板***
QString getBaseboardSerialnumber()
{
     return getInfo("wmic baseboard get serialnumber");
}
//查询BIOS***
QString getBiosSerialnumber()
{
     return getInfo("wmic bios get serialnumber");
}
//查询主板唯一标识
QString getBaseboardUuid()
{
     return getInfo("wmic csproduct get uuid");
}
#endif // GETINFOMATION_H
