#ifndef CPROGRAMPROJECTVIEW_H
#define CPROGRAMPROJECTVIEW_H

#include <QWidget>

namespace Ui {
class CProgramProjectView;
}

class QToolButton;
class CProject;
class CProjectModel;
class CProgramProjectView : public QWidget
{
    Q_OBJECT
public:
    explicit CProgramProjectView(QWidget *parent = 0);
    ~CProgramProjectView();
    void setProject(CProject* project);
    CProject* project();
private slots:
    void on_pushButtonSet_clicked();
    void addCanport();
    void delCanport();
private:
    Ui::CProgramProjectView *ui;
    CProjectModel* m_model;
};

#endif // CPROGRAMPROJECTVIEW_H
