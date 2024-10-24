#ifndef COBJECT_H
#define COBJECT_H

#include "objectstatusenum.h"

#include <QObject>
#include <QList>
#include <QHash>
#include <QSet>
#include <QVariant>

#define OBJECT_VALUE_PIXMAP "pixmap"
#define OBJECT_VALUE_DISPLAY_NAME "_A_display_name"
#define OBJECT_VALUE_ADDRESS "_A_address"
#define OBJECT_VALUE_DESCRIPTION "_A_description"
#define OBJECT_VALUE_ONLINE_COUNT "online_count"
#define OBJECT_VALUE_ONLINE "object_online"

class CDBSqlite;
class CObject : public QObject
{
    Q_OBJECT
public:

    enum EditState{ES_None = 0, ES_New = 1, ES_Modify = 2, ES_Deleted = 4};
    enum ObjectType{OT_AddChildren, OT_RemoveChildren};
    Q_DECLARE_FLAGS(EditStatus, EditState)
    Q_FLAGS(EditStatus)

    explicit CObject(const int keyId = 0);
    virtual ~CObject();
    virtual QString type() { return metaObject()->className(); }
    static int currentId() { return m_currentKeyId; }
    static QHash<short, unsigned char> compareStatusTable(const QHash<short, unsigned char>& oldStatusTable, const QHash<short, unsigned char>& newStatusTable);
    int keyId() const { return m_keyId; }

    virtual int save(CDBSqlite *db) { Q_UNUSED(db); return 0; }
    virtual CObject* parent() { return 0; }
    virtual CObject* child(const int index) { Q_UNUSED(index); return 0; }
    virtual QList<CObject*> childs() { QList<CObject*> t; return t; }

    virtual QString codeStr() const { return tr("Object%1").arg(keyId()); }
    virtual QString typeStr() const { return tr("Object"); }
    virtual QString descriptionStr() const { return QString(); }

    inline void setIsUndefined(bool b) { m_isUndefined = b; }
    inline bool isUndefined() const { return m_isUndefined; }
    CObject::EditStatus editStatus() const { return m_editStatus; }
    void setEditStatus(const EditStatus status);
    void setNew();
    void setModify();
    void setDeleted();
    void setNone();
    bool isNew() const { return m_editStatus & ES_New; }
    bool isModify() const { return m_editStatus & ES_Modify; }
    bool isDeleted() const { return m_editStatus & ES_Deleted; }
    bool isNone() const { return m_editStatus ? true : false; }
    bool setValue(QString name, QVariant value);
    QVariant value(const QString name) const;
    void setStatus(short status, unsigned char value, unsigned int time = 0);
    unsigned char getStatus(short status) const;
    unsigned int getStatusTime(short status) const;
    bool isStartStatus() const;
    bool isFaultStatus() const;
    bool isEmergencyStatus() const;
    bool isFireStatus() const;
    bool hasStatus() const { return !m_objectStatus.isEmpty(); }
    void setStatusTable(const QHash<short, unsigned char>& statusTable, const QHash<short, unsigned int>& statusTime = QHash<short, unsigned int>());
    QHash<short, unsigned char> getStatusTable() const { return m_objectStatus; }
    QHash<short, unsigned int> getStatusTime() const { return m_objectStatusTime; }
    QString getStatusDes();
    QString getCommunicationStatus();
    void clearStatus(bool reserveLogin = false);
    void setKeyId(const int id) { m_keyId = id; calcCurrentId(id); }
signals:
    void editStatusChanged(CObject::EditStatus status);
    void objectStatusChanged(short status, unsigned char oldValue, unsigned char newValue, unsigned int time);
    void valueChanged(QString& name, QVariant& oldValue, QVariant& newValue);
    void objectChanged(CObject::ObjectType type, CObject* object);
    void destrory(CObject* object);
protected:
    virtual bool beforeSetValue(QString& name, QVariant& value) { Q_UNUSED(name); Q_UNUSED(value); return true; }
    virtual void afterSetValue(QString& name, QVariant& oldValue, QVariant& newValue) { Q_UNUSED(name); Q_UNUSED(oldValue); Q_UNUSED(newValue); setModify(); }
    virtual QVariant afterGetValue(QString name, QVariant value) const { Q_UNUSED(name); return value; }
    virtual bool beforeSetStatus(short status, unsigned char value) { Q_UNUSED(status); Q_UNUSED(value); return true; }
    virtual void afterSetStatus(short status, unsigned char value) { Q_UNUSED(status); Q_UNUSED(value); }
    virtual unsigned char afterGetStatus(short status, unsigned char value) const { Q_UNUSED(status); return value; }
private:

    void calcCurrentId(const int n) { m_currentKeyId = m_currentKeyId > n ? m_currentKeyId : n; }
protected:
    QHash<QString, QVariant> m_value;
    QHash<short, unsigned char> m_objectStatus;
    QHash<short, unsigned int> m_objectStatusTime;
    EditStatus m_editStatus;
private:
    static int m_currentKeyId;
    int m_keyId;
    bool m_isUndefined;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CObject::EditStatus)

#endif // COBJECT_H
