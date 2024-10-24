#ifndef CDESIGNTREEMODEL_H
#define CDESIGNTREEMODEL_H

#include "cmodel.h"

class CController;
class CDesignTreeModel : public CModel
{
    Q_OBJECT
public:
    explicit CDesignTreeModel(QObject *parent = 0);
    CDesignTreeModel(CController* controller, bool track = true, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setController(CController* controller);
    CController* controller(){ return m_controller; }
protected slots:
    virtual void objectDestroy(CObject* object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    QVariant getDisplayData(const QModelIndex &index) const;
//    QVariant getEditData(const QModelIndex &index) const;
    QVariant getDecorationData(const QModelIndex &index) const;
    QVariant getTextFontData(const QModelIndex &index) const;
    QVariant getTextColorData(const QModelIndex &index) const;
//    bool setEditData(const QModelIndex &index, const QVariant &value);
private:
    CController* m_controller;
};


#endif // CDESIGNTREEMODEL_H
