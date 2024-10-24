#ifndef CCANPORTMODEL_H
#define CCANPORTMODEL_H

#include "cmodel.h"

#define CANPORT_COLUMN_NAME 0
#define CANPORT_COLUMN_NUMBER 1
#define CANPORT_COLUMN_ONLINE 2

class CCanport;
class CCanportModel : public CModel
{
    Q_OBJECT
public:
    explicit CCanportModel(QObject *parent = 0);
    ~CCanportModel();
    CCanportModel(CCanport *canport, bool track = true, QObject *parent = 0);
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setCanport(CCanport* canport);
    CCanport* canport(){ return m_canport; }
protected slots:
    void objectChanged(CObject::ObjectType type, CObject* object);
    void objectDestroy(CObject *object);
protected:
    QVariant data(const QModelIndex &index, int role) const;
private:
    void initObject();
    void calcObject();
private slots:
    void updateDeviceType();
    void updateOnline();
private:
    CCanport* m_canport;
    CObject* m_distributionCount;
    CObject* m_loopCount;
    QHash<int, CObject*> m_deviceCount;
    CObject* m_powerCount;
    CObject* m_alarmPointCount;
};

#endif // CCANPORTMODEL_H
