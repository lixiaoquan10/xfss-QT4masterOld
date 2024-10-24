#ifndef CPROGRAMCONTROLLERVIEW_H
#define CPROGRAMCONTROLLERVIEW_H

#include <QWidget>

namespace Ui {
class CProgramControllerView;
}

class QToolButton;
class CController;
class CControllerModel;
class CProgramControllerView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramControllerView(QWidget *parent = 0);
    ~CProgramControllerView();
    void setController(CController* controller);
    CController* controller();
private slots:
    void on_pushButtonSet_clicked();
    void addCanport();
    void delCanport();
private:
    Ui::CProgramControllerView *ui;
    CControllerModel* m_model;
};

#endif // CPROGRAMCONTROLLERVIEW_H
