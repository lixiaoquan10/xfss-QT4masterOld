#ifndef CPROGRAMCANPORTVIEW_H
#define CPROGRAMCANPORTVIEW_H

#include <QWidget>

namespace Ui {
class CProgramCanportView;
}

class CCanport;
class CCanportModel;
class CProgramCanportView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramCanportView(QWidget *parent = 0);
    ~CProgramCanportView();
    void setCanport(CCanport* canport);
    CCanport* canport();
private slots:
    void on_pushButtonSet_clicked();
private:
    Ui::CProgramCanportView *ui;
    CCanportModel* m_model;
};

#endif // CPROGRAMCANPORTVIEW_H
