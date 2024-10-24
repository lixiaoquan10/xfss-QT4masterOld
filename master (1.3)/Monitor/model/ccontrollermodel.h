#ifndef CCONTROLLERMODEL_H
#define CCONTROLLERMODEL_H

#include "cmodel.h"

#define CONTROLLER_COLUMN_NAME 0
#define CONTROLLER_COLUMN_NUMBER 1
#define CONTROLLER_COLUMN_ONLINE 2

class CController;
class CControllerModel : public CModel
{
    Q_OBJECT
public:
    explicit CControllerModel(QObject *parent = 0);
    ~CControllerModel();
    CControllerModel(CController *project, bool track = true, QObject *parent = 0);
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setController(CController* project);
    CController* controller(){ return m_controller; }
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
    CController* m_controller;
    CObject* m_distributionCount;
    CObject* m_loopCount;
    QHash<int, CObject*> m_deviceCount;
    CObject* m_powerCount;
    CObject* m_alarmPointCount;
};

#endif // CCONTROLLERMODEL_H
