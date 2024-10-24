#ifndef CLOG_H
#define CLOG_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QDateTime>

class CMsgLog;
class CLog
{
public:
    explicit CLog(const QString& dbName);
    ~CLog();
    bool addLog(CMsgLog* log);
    QList<CMsgLog*> getLog(const QDateTime& beginTime, const QDateTime& endTime, const QString& type = QString(), const int page = 0, const int number = 0) const;
private:
    bool createDB();
    bool checkDB(const QString& dbName);
    void backupDB(const QString& dbName);
    void openDB(const QString& dbName);
    void closeDB();
private:
    QString m_dbName;
    QSqlDatabase m_db;
};

#endif // CLOG_H
