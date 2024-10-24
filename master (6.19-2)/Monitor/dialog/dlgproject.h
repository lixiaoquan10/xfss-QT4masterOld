#ifndef DLGPROJECT_H
#define DLGPROJECT_H

#include <QDialog>

namespace Ui {
class dlgProject;
}

class QModelIndex;
class CDlgProjectTableModel;
class dlgProject : public QDialog
{
    Q_OBJECT

public:
    explicit dlgProject(QWidget *parent = 0);
    ~dlgProject();
    QString projectName() const { return m_projectName; }
    QString currentSelected() const;

private slots:
    void on_btnImport_clicked();

    void on_btnDelete_clicked();

    void on_btnOpen_clicked();

    void on_btnExport_clicked();

    void dbclickTable(const QModelIndex& index);

private:
    Ui::dlgProject *ui;
    CDlgProjectTableModel* m_model;
    QString m_projectName;
};

#endif // DLGPROJECT_H
