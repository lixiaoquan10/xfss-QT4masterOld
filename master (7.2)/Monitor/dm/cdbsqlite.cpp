#include "cglobal.h"

CDBSqlite::CDBSqlite(const QString &dbName, QObject *parent) :
    QObject(parent), m_dbName(dbName), m_currentVersion(1)
{

}

CDBSqlite::~CDBSqlite()
{
    close();
}

bool CDBSqlite::open()
{
    qDebug() << "CDBSqlite::open"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(m_db.isOpen())
        return true;
    if(m_dbName.isEmpty())
        return false;
    bool exists = QFile::exists(m_dbName);
    m_db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), m_dbName);
    m_db.setDatabaseName(m_dbName);
    if (!m_db.open()) {
        qDebug() << tr("Cannot open database:") << m_db.lastError();
        return false;
    }
    m_db.exec("PRAGMA cache_size = 100000");
    m_db.exec("PRAGMA synchronous = NULL");
    m_db.exec("PRAGMA temp_store = MEMORY");
    if(!exists)
        createTableDatainfo();
    return m_db.isOpen();
}

void CDBSqlite::close()
{
    qDebug() << "CDBSqlite::close"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if (m_db.isOpen())
        m_db.close();
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_dbName);
}

QSqlQuery CDBSqlite::query(const QString &sql)
{
    qDebug() << "CDBSqlite::query"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_db.isOpen())
        return QSqlQuery();
    return QSqlQuery(sql, m_db);
}

bool CDBSqlite::transaction()
{
    qDebug() << "CDBSqlite::transaction"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_db.isOpen())
        return false;
    return m_db.transaction();
}

bool CDBSqlite::rollback()
{
    qDebug() << "CDBSqlite::rollback"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_db.isOpen())
        return false;
    return m_db.rollback();
}

bool CDBSqlite::commit()
{
    qDebug() << "CDBSqlite::commit"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    if(!m_db.isOpen())
        return false;
    return m_db.commit();
}

QString CDBSqlite::dbInformation()
{
    qDebug() << "CDBSqlite::dbInformation"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("SELECT information FROM datainfo LIMIT 1");
    int filedInformation = query.record().indexOf("information");
    QString information;
    if(query.next())
        information = query.value(filedInformation).toString();
    return information;
}

int CDBSqlite::setDbInformation(const QString &info)
{
    qDebug() << "CDBSqlite::setDbInformation"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("UPDATE datainfo SET information = :information");
    query.bindValue(":information", info);
    if(!query.exec())
        return -1;
    return 0;
}

int CDBSqlite::dbVersion()
{
    qDebug() << "CDBSqlite::dbVersion"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("SELECT version FROM datainfo LIMIT 1");
    int filedVersion = query.record().indexOf("version");
    int version = 0;
    if(query.next())
        version = query.value(filedVersion).toInt();
    return version;
}

int CDBSqlite::setDbVersion(const int version)
{
    qDebug() << "CDBSqlite::setDbVersion"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("UPDATE datainfo SET version = :version");
    query.bindValue(":version", version);
    if(!query.exec())
        return -1;
    return 0;
}

QString CDBSqlite::projectName()
{
    qDebug() << "CDBSqlite::projectName"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("SELECT project_name FROM project LIMIT 1");
    int filedProjectName = query.record().indexOf("project_name");
    QString projectName;
    if(query.next())
        projectName = query.value(filedProjectName).toString();
    return projectName;
}

QDateTime CDBSqlite::createTime()
{
    qDebug() << "CDBSqlite::createTime"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("SELECT createtime FROM datainfo LIMIT 1");
    int filedCreateTime = query.record().indexOf("createtime");
    QDateTime createTime;
    if(query.next())
        createTime = query.value(filedCreateTime).toDateTime();
    return createTime;
}

int CDBSqlite::setCreateTime(const QDateTime &time)
{
    qDebug() << "CDBSqlite::setCreateTime"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("UPDATE datainfo SET createtime = :createtime");
    query.bindValue(":createtime", time);
    if(!query.exec())
        return -1;
    return 0;
}

QString CDBSqlite::createId()
{
    qDebug() << "CDBSqlite::createId"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("SELECT createid FROM datainfo LIMIT 1");
    int filedCreateId = query.record().indexOf("createid");
    QString createId;
    if(query.next())
        createId = query.value(filedCreateId).toString();
    return createId;
}

int CDBSqlite::setCreateId(const QString &id)
{
    qDebug() << "CDBSqlite::setCreateId"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("UPDATE datainfo SET createid = :createid");
    query.bindValue(":createid", id);
    if(!query.exec())
        return -1;
    return 0;
}

QDateTime CDBSqlite::updateTime()
{
    qDebug() << "CDBSqlite::updateTime"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("SELECT updatetime FROM datainfo LIMIT 1");
    int filedUpdateTime = query.record().indexOf("updatetime");
    QDateTime updateTime;
    if(query.next())
        updateTime = query.value(filedUpdateTime).toDateTime();
    return updateTime;
}

int CDBSqlite::setUpdateTime(const QDateTime &time)
{
    qDebug() << "CDBSqlite::setUpdateTime"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("UPDATE datainfo SET updatetime = :updatetime");
    query.bindValue(":updatetime", time);
    if(!query.exec())
        return -1;
    return 0;
}

QString CDBSqlite::updateId()
{
    qDebug() << "CDBSqlite::updateId"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("SELECT updateid FROM datainfo LIMIT 1");
    int filedUpdateId = query.record().indexOf("updateid");
    QString updateId;
    if(query.next())
        updateId = query.value(filedUpdateId).toString();
    return updateId;
}

int CDBSqlite::setUpdateId(const QString &id)
{
    qDebug() << "CDBSqlite::setUpdateId"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query = this->query("UPDATE datainfo SET updateid = :updateid");
    query.bindValue(":updateid", id);
    if(!query.exec())
        return -1;
    return 0;
}

void CDBSqlite::createTableDatainfo()
{
    qDebug() << "CDBSqlite::createTableDatainfo"
             << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QSqlQuery query(m_db);
    if (!query.exec(
                "CREATE TABLE IF NOT EXISTS datainfo ("
                    "information VARCHAR(255),"
                    "version INTEGER,"
                    "createtime DATETIME,"
                    "createid VARCHAR(255),"
                    "updatetime DATETIME,"
                    "updateid VARCHAR(255)"
                ");"
    )) {
        qDebug() << tr("Cannot create table datainfo:") << query.lastError();
        return;
    }

    QSqlQuery query2 = this->query("REPLACE INTO datainfo (information, version) "
                                "VALUES (:information, :version)");
    query2.bindValue(":information", "CFG");
    query2.bindValue(":version", m_currentVersion);
    query2.exec();
}
