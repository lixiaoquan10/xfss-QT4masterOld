#ifndef CONTROLLERMODEL_H
#define CONTROLLERMODEL_H

#include "cmodel.h"

#define CONTROLLER_COLUMN_ADDRESS 0
#define CONTROLLER_COLUMN_NAME 1
#define CONTROLLER_COLUMN_DESCRIPTION 2

class CController;
class CControllerModel : public CModel
{
    Q_OBJECT
public:
    explicit CControllerModel(QObject *parent = 0);
    CControllerModel(CController* controller, bool track = true, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setController(CController* controller);
    CController* controller(){ return m_controller; }
protected slots:
    void objectChanged(CObject::ObjectType type, CObject* object);
    void objectDestroy(CObject *object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const;

private:
    CController* m_controller;
};

#endif // CONTROLLERMODEL_H
