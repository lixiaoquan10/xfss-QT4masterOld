#ifndef DLGHANDFIREVIEW_H
#define DLGHANDFIREVIEW_H

#include <QDialog>

namespace Ui {
class dlgHandFireView;
}

class dlgHandFireView : public QDialog
{
    Q_OBJECT
public:
    explicit dlgHandFireView(QWidget *parent = 0);
    ~dlgHandFireView();
private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_fire_clicked();

private:
    Ui::dlgHandFireView *ui;
};

#endif // DLGHANDFIREVIEW_H
