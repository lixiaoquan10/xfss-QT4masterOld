#ifndef CDESIGNTREEMODEL_H
#define CDESIGNTREEMODEL_H

#include "cmodel.h"

class CProject;
class CDesignTreeModel : public CModel
{
    Q_OBJECT
public:
    explicit CDesignTreeModel(QObject *parent = 0);
    CDesignTreeModel(CProject* project, bool track = true, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setProject(CProject* project);
    CProject* project(){ return m_project; }
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
    CProject* m_project;
};


#endif // CDESIGNTREEMODEL_H
