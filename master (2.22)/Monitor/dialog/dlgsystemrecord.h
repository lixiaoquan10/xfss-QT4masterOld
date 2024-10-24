#ifndef DLGSYSTEMRECORD_H
#define DLGSYSTEMRECORD_H

#include <QDialog>

class dlgSystemRecordModel;
class CMsgLog;

namespace Ui {
class dlgSystemRecord;
}

class dlgSystemRecord : public QDialog
{
    Q_OBJECT

public:
    explicit dlgSystemRecord(QWidget *parent = 0);
    ~dlgSystemRecord();
    void weekrecord();
    void dayrecord();
    void customQueryrecord();
    void setPage(int page);
    void updateRecord();
    dlgSystemRecordModel *m_Model;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_before_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_before_query_clicked();

    void on_pushButton_next_query_clicked();

private:
    Ui::dlgSystemRecord *ui;
    int m_page;
    int m_mode;
};

#endif // DLGSYSTEMRECORD_H
