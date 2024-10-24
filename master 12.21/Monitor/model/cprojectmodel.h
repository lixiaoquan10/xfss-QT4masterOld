#ifndef CPROJECTMODEL_H
#define CPROJECTMODEL_H

#include "cmodel.h"

#define PROJECT_COLUMN_ADDRESS 0
#define PROJECT_COLUMN_NAME 1
#define PROJECT_COLUMN_DESCRIPTION 2

class CProject;
class CProjectModel : public CModel
{
    Q_OBJECT
public:
    explicit CProjectModel(QObject *parent = 0);
    CProjectModel(CProject* project, bool track = true, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setProject(CProject* project);
    CProject* project(){ return m_project; }
protected slots:
    void objectChanged(CObject::ObjectType type, CObject* object);
    void objectDestroy(CObject *object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    QVariant getDisplayData(const QModelIndex &index) const;
//    QVariant getTextColorData(const QModelIndex &index) const;
//    QVariant getEditData(const QModelIndex &index) const;
//    bool setEditData(const QModelIndex &index, const QVariant &value);
private:
    CProject* m_project;
};

#endif // CPROJECTMODEL_H
