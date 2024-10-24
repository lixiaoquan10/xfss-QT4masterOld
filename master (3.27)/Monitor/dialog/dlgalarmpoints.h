#ifndef DLGALARMPOINTS_H
#define DLGALARMPOINTS_H

#include <QDialog>
#include <QList>

namespace Ui {
class dlgAlarmPoints;
}

class CObject;
class CProject;
class CAlarmPoint;
class CAlarmPointsFilterTableModel;
class dlgAlarmPoints : public QDialog
{
    Q_OBJECT

public:
    explicit dlgAlarmPoints(QWidget *parent = 0);
    ~dlgAlarmPoints();
    void closeEvent(QCloseEvent *event);
    QList<CAlarmPoint*> currentSelected();
private:
    void initData();
    void loadController(CProject* project = 0);
private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
    void controllerChanged(const int i);
    void addressChanged(const QString address);
    void startDrag(Qt::DropActions supportedActions);
    void addRemovedObject(CObject* object);
    void removeAddObject(CObject* object);
private:
    static QPixmap* m_pixmap;
    Ui::dlgAlarmPoints *ui;
    CAlarmPointsFilterTableModel* m_model;
};

#endif // DLGALARMPOINTS_H
