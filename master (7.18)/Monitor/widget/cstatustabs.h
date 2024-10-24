#ifndef CSTATUSTABS_H
#define CSTATUSTABS_H

#include <QWidget>
#include <QStringList>
#include <QHash>
class CInformationModel;
class QTabWidget;
class CTableView;
class QModelIndex;

class CStatusTabs : public QWidget
{
    Q_OBJECT
public:
    explicit CStatusTabs(QWidget *parent = 0);
    ~CStatusTabs();
    QTabWidget *m_tabWidget;
    QStringList m_tabList;
    QHash<QString, CTableView*> m_tableView;
    QHash<QString, CInformationModel*> m_informationModel;
signals:

public slots:
    void slot_tableviewClicked(const QModelIndex &index);
private:
    void createTab(const QString &name);
};

#endif // CSTATUSTABS_H
