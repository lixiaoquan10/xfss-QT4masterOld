#ifndef CCONTROLLERTREEMODEL_H
#define CCONTROLLERTREEMODEL_H

#include "cmodel.h"

class CController;
class CControllerTreeModel : public CModel
{
    Q_OBJECT
public:
    explicit CControllerTreeModel(QObject *parent = 0);
    CControllerTreeModel(CController* controller, bool track = true, QObject *parent = 0);

    void setController(CController* controller);
    CController* controller(){ return m_controller; }
protected slots:
    virtual void objectDestroy(CObject* object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    void afterLoadChildren(CObject *object, QModelIndex &parent);
    QVariant getDisplayData(const QModelIndex &index) const;
    QVariant getDecorationData(const QModelIndex &index) const;
    QVariant getTextColorData(const QModelIndex &index) const;
//    QVariant getToolTipData(const QModelIndex &index) const;
    QVariant getTextFontData(const QModelIndex &index) const;
private:
    CController* m_controller;
};

#endif // CCONTROLLERTREEMODEL_H
