#ifndef CPROJECTTREEMODEL_H
#define CPROJECTTREEMODEL_H

#include "cmodel.h"

class CProject;
class CProjectTreeModel : public CModel
{
    Q_OBJECT
public:
    explicit CProjectTreeModel(QObject *parent = 0);
    CProjectTreeModel(CProject* project, bool track = true, QObject *parent = 0);

    void setProject(CProject* project);
    CProject* project(){ return m_project; }
protected slots:
    virtual void objectDestroy(CObject* object);
protected:
    bool beforeLoadChildren(CObject *object, QModelIndex &parent);
    void afterLoadChildren(CObject *object, QModelIndex &parent);
    QVariant getDisplayData(const QModelIndex &index) const;
    QVariant getDecorationData(const QModelIndex &index) const;
    QVariant getTextColorData(const QModelIndex &index) const;
    QVariant getToolTipData(const QModelIndex &index) const;
    QVariant getTextFontData(const QModelIndex &index) const;
private:
    CProject* m_project;
};

#endif // CPROJECTTREEMODEL_H
