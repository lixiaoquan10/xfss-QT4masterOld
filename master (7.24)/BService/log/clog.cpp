#include "clog.h"
#include "cglobal.h"
#include "struct.h"
#include "struct/cmsglog.h"
#include "dialog/dlginformationwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>

CLog::CLog(const QString &dbName)
{
    if(!checkDB(dbName))
    {
        qDebug()<<"Check Database fault!";
        backupDB(dbName);
    }
    openDB(dbName);
}

CLog::~CLog()
{
    closeDB();
}

bool CLog::addLog(CMsgLog* log)
{
    qDebug() << "CLog::addLog" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    int64_t count;
    QSqlQuery query(m_db);
    //获取事件记录条数
    query.prepare("SELECT COUNT(*) FROM log");
    query.exec();

    if (query.next()) {
        count = query.value(0).toInt();
    }

    //判断事件记录长度是否超出
    if(count >= 30000)
    {
        query.prepare("DELETE FROM log WHERE EventTypeID = (SELECT EventTypeID FROM log ORDER BY Time ASC LIMIT 1)");
        query.exec();
    }
    //插入事件记录
    query.prepare("INSERT INTO log (User, Time, Remark, Type, CanPort, CanDeviceAddress, Loop, LampDeviceAddress, Area, Location) "
               "VALUES (:User, :Time, :Remark, :Type, :CanPort, :CanDeviceAddress, :Loop, :LampDeviceAddress, :Area, :Location)");
    query.bindValue(":User", log->User);
    query.bindValue(":Time", log->Time);
    query.bindValue(":Remark", log->Remark);
    query.bindValue(":Type", log->Type);
    if(log->Canport)
        query.bindValue(":CanPort", log->Canport);
    if(log->CanDeviceAddress)
        query.bindValue(":CanDeviceAddress", log->CanDeviceAddress);
    if(log->Loop)
        query.bindValue(":Loop", log->Loop);
    if(log->LampDeviceAddress)
        query.bindValue(":LampDeviceAddress", log->LampDeviceAddress);
    query.bindValue(":Area", log->Area);
    query.bindValue(":Location", log->Location);

    if (!query.exec()) {
        qDebug() << "Cannot add log:" << query.lastError();
        if(!CGlobal::instance()->DlgInformationWindow())
            new dlgInformationWindow();
        CGlobal::instance()->DlgInformationWindow()->setDescription(QString("<h2><font color=red>%1<br/>%2<br/>%3<br/>%4</font></h2>")
                                                                    .arg("日志数据无法添加，请联系系统管理员！")
                                                                    .arg(QObject::tr("错误代码：%1").arg(query.lastError().number()))
                                                                    .arg(query.lastError().driverText())
                                                                    .arg(query.lastError().databaseText()));
        CGlobal::instance()->DlgInformationWindow()->show();
        query.clear();
        closeDB();
        backupDB(m_dbName);
        openDB(m_dbName);
        return false;
    }
    return true;
}

QList<CMsgLog *> CLog::getLog(const QDateTime &beginTime, const QDateTime &endTime, const QString &type, const int page, const int number) const
{
    qDebug() << "CLog::getLog" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QList<CMsgLog*> list;

    QString sql = QString("SELECT EventTypeID, User, Time, Remark, Type, CanPort, CanDeviceAddress, Loop, LampDeviceAddress, Area, Location"
                          " FROM log WHERE time > '%1' AND time < '%2' ")
            .arg(beginTime.toString("yyyy-MM-dd")).arg(endTime.addDays(1).toString("yyyy-MM-dd"));
    if(!type.isEmpty())
    {
        sql.append(QString(" AND type = '%1'").arg(type));
    }
    sql.append(QString(" ORDER BY Time DESC LIMIT '%1', '%2' ").arg((page-1)*number).arg(number));
    qDebug(sql.toAscii().data());
    QSqlQuery query(sql, m_db);
    QSqlRecord record = query.record();
    CMsgLog* tmp;
    int filedId = record.indexOf("EventTypeID");
    int filedUser = record.indexOf("User");
    int filedTime = record.indexOf("Time");
    int filedRemark = record.indexOf("Remark");
    int filedType = record.indexOf("Type");
    int filedCanPort = record.indexOf("CanPort");
    int filedCanDeviceAddress = record.indexOf("CanDeviceAddress");
    int filedLoop = record.indexOf("Loop");
    int filedLampDeviceAddress = record.indexOf("LampDeviceAddress");
    int filedArea = record.indexOf("Area");
    int filedLocation = record.indexOf("Location");

    while(query.next())
    {
        tmp = new CMsgLog();
        tmp->EventTypeID = query.value(filedId).toInt();
        tmp->User = query.value(filedUser).toString();
        tmp->Time = query.value(filedTime).toDateTime();
        tmp->Remark = query.value(filedRemark).toString();
        tmp->Type = query.value(filedType).toString();
        tmp->Canport = query.value(filedCanPort).toInt();
        tmp->CanDeviceAddress = query.value(filedCanDeviceAddress).toInt();
        tmp->Loop = query.value(filedLoop).toInt();
        tmp->LampDeviceAddress = query.value(filedLampDeviceAddress).toInt();
        tmp->Area = query.value(filedArea).toString();
        tmp->Location = query.value(filedLocation).toString();

        list.append(tmp);

    }
    return list;
}

bool CLog::createDB()
{
    qDebug() << "CLog::createDB" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query(m_db);
    if (!query.exec(
                "CREATE TABLE IF NOT EXISTS log ("
                    "EventTypeID INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "User VARCHAR(20),"
                    "Time DATETIME,"
                    "Remark VARCHAR(200),"
                    "Type VARCHAR(20),"
                    "CanPort INTEGER,"
                    "CanDeviceAddress INTEGER,"
                    "Loop INTEGER,"
                    "LampDeviceAddress INTEGER,"
                    "Area VARCHAR,"
                    "Location VARCHAR"
                ");"
    )) {
        qDebug() << "Cannot create table log:" << query.lastError();
        if(!CGlobal::instance()->DlgInformationWindow())
            new dlgInformationWindow();
        CGlobal::instance()->DlgInformationWindow()->setDescription(QString("<h2><font color=red>%1<br/>%2<br/>%3<br/>%4</font></h2>")
                                                                    .arg("日志数据无法添加，请联系系统管理员！")
                                                                    .arg(QObject::tr("错误代码：%1").arg(query.lastError().number()))
                                                                    .arg(query.lastError().driverText())
                                                                    .arg(query.lastError().databaseText()));
        CGlobal::instance()->DlgInformationWindow()->show();
        return false;
    }
    return true;
}

bool CLog::checkDB(const QString &dbName)
{
    qDebug() << "CLog::checkDB" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFileInfo fInfo(dbName);
    if(fInfo.size() < 104857600)
        return true;
    else
        return false;
}

void CLog::backupDB(const QString &dbName)
{
    qDebug() << "CLog::backupDB" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QFile file(dbName);
    QFileInfo fileInfo(dbName);
    QString bakName = fileInfo.path() + "/" + fileInfo.baseName() + QDateTime::currentDateTime().toString("_yyyyMMdd_hhmmss_zzz.") + fileInfo.suffix();
    qDebug()<<bakName<<file.rename(bakName);
}

void CLog::openDB(const QString& dbName)
{
    qDebug() << "CLog::openDB" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_dbName = dbName;
    bool exists = QFile::exists(m_dbName);
    m_db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), m_dbName);
    m_db.setDatabaseName(m_dbName);
    if (!m_db.open()) {
        qDebug() << "Cannot open database:" << m_db.lastError();
        if(!CGlobal::instance()->DlgInformationWindow())
            new dlgInformationWindow();

        CGlobal::instance()->DlgInformationWindow()->setDescription(QString("<h2><font color=red>%1<br/>%2<br/>%3<br/>%4</font></h2>")
                                                                    .arg("日志数据无法添加，请联系系统管理员！")
                                                                    .arg(QObject::tr("错误代码：%1").arg(m_db.lastError().number()))
                                                                    .arg(m_db.lastError().driverText())
                                                                    .arg(m_db.lastError().databaseText()));
        CGlobal::instance()->DlgInformationWindow()->show();
        return;
    }
    m_db.exec("PRAGMA cache_size = 30000");
    m_db.exec("PRAGMA synchronous = OFF");
    m_db.exec("PRAGMA temp_store = MEMORY");
    if(!exists)
        createDB();
}

void CLog::closeDB()
{
    qDebug() << "CLog::closeDB" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_db.isOpen())
        m_db.close();
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_dbName);
}
