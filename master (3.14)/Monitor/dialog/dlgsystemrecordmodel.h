#ifndef DLGSYSTEMRECORDMODEL_H
#define DLGSYSTEMRECORDMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QDebug>

class CMsgLog;
class dlgSystemRecordModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit dlgSystemRecordModel(QObject *parent = 0);
    ~dlgSystemRecordModel();

    bool addData(CMsgLog *MsgLog);
    void removeData();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    void setHeaderList(QStringList Header){m_HeaderTemplet=Header;}
    QStringList m_HeaderTemplet;
    QList<CMsgLog *> m_MsgLog;
};

#endif // DLGSYSTEMRECORDMODEL_H
