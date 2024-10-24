#ifndef CDBSQLITE_H
#define CDBSQLITE_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QSqlDatabase>

class CDBSqlite : public QObject
{
    Q_OBJECT
public:
    explicit CDBSqlite(const QString& dbName, QObject *parent = 0);
    ~CDBSqlite();

    bool open();
    bool isOpen() { return m_db.isOpen(); }
    void close();
    QSqlQuery query(const QString& sql);
    bool transaction();
    bool rollback();
    bool commit();
    QString dbName() { return m_dbName; }
    int currentVersion() { return m_currentVersion; }
    QString dbInformation();
    int setDbInformation(const QString& info);
    int dbVersion();
    int setDbVersion(const int version);
    QString projectName();
    QDateTime createTime();
    int setCreateTime(const QDateTime& time);
    QString createId();
    int setCreateId(const QString& id);
    QDateTime updateTime();
    int setUpdateTime(const QDateTime& time);
    QString updateId();
    int setUpdateId(const QString& id);
private:
    void createTableDatainfo();

private:
    QString m_dbName;
    int m_currentVersion;
    QSqlDatabase m_db;
};

#endif // CDBSQLITE_H
