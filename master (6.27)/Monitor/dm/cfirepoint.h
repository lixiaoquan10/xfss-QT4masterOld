#ifndef CFIREPOINT_H
#define CFIREPOINT_H

#include "cobject.h"

#define FIREPOINT_VALUE_ID "firepoint_id"
#define FIREPOINT_VALUE_ADDRESS "firepoint_address"
#define FIREPOINT_VALUE_DEVICEADDRESS "firepoint_deviceaddress"
#define FIREPOINT_VALUE_LOOPADDRESS "firePoint_loopaddress"
#define FIREPOINT_VALUE_AREA "firePoint_area"
#define FIREPOINT_VALUE_LOCATION "firePoint_location"
#define FIREPOINT_VALUE_LAYOUTPAGEID "firePoint_layoutpageid"
#define FIREPOINT_VALUE_CX "firePoint_cx"
#define FIREPOINT_VALUE_CY "firePoint_cy"
#define FIREPOINT_VALUE_WIDTH "firePoint_width"
#define FIREPOINT_VALUE_HEIGHT "firePoint_height"
#define FIREPOINT_VALUE_EMERGENCY "firePoint_emergency"

class CController;
class CFirePoint : public CObject
{
    Q_OBJECT
public:
    explicit CFirePoint(const int keyId = 0);
    CFirePoint(CController *controller, const int keyId = 0);
    ~CFirePoint();

    static void createTable(CDBSqlite* db);
    static void clearTable(CDBSqlite* db);
    static int loadFirePoint(CDBSqlite *db, CController* controller);
    int save(CDBSqlite *db) { return saveFirePoint(db); }
    CObject* parent();
    QString codeStr() const;
    QString typeStr() const { return "火警点"; }

    bool setFirePointId(int id) { return setFirePointValue(FIREPOINT_VALUE_ID, id); }
    int firePointId() const { return firePointValue(FIREPOINT_VALUE_ID).toInt(); }
    int firePointDeviceAddress() const { return firePointValue(FIREPOINT_VALUE_DEVICEADDRESS).toInt(); }
    int firePointLoopAddress() const { return firePointValue(FIREPOINT_VALUE_LOOPADDRESS).toInt(); }
    int firePointAddress() const { return firePointValue(FIREPOINT_VALUE_ADDRESS).toInt(); }
    void setController(CController* controller) { m_controller = controller; }
    CController* controller() const { return m_controller; }
    bool setAddress(const int address);
    int address() const { return firePointValue(FIREPOINT_VALUE_ADDRESS).toInt(); }

    bool setFirePointValue(QString name, QVariant value);
    QVariant firePointValue(const QString name) const;
    void setSimulateEmergency(bool enable) { m_isSimulateEmergency = enable; }
    bool isSimulateEmergency() { return m_isSimulateEmergency; }

    void setEmergency(bool enable) { m_isEmergency = enable; }
    bool isEmergency() { return m_isEmergency; }

signals:
    void addressChanged(QString& oldAddress, QString& newAddress);
protected:
    void afterSetValue(QString& name, QVariant& oldValue, QVariant& newValue);
    QVariant afterGetValue(QString name, QVariant value) const;
    void afterSetStatus(short status, unsigned char value);
private:
    void initFirePoint();
    int saveFirePoint(CDBSqlite* db);
public:
    const static QString type_s;
private:
    CController* m_controller;
    QHash<QString, QVariant> m_firePointValue;
    bool m_isSimulateEmergency;
    bool m_isEmergency;
};


#endif // CFIREPOINT_H
