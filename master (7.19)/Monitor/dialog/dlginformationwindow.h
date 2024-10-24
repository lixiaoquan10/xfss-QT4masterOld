#ifndef DLGINFORMATIONWINDOW_H
#define DLGINFORMATIONWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QProgressBar>
#include <QGridLayout>

class dlgInformationWindow : public QDialog
{
    Q_OBJECT
public:
    explicit dlgInformationWindow(QWidget *parent = 0);
    ~dlgInformationWindow();

    void setMovie(QString filePath);
    void setDescription(QString description);
    void hideCloseButton();
    void showCloseButton();
    void setLiftTime(QString str);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
public slots:
    void slot_close();
private:
    QMovie *movie;
    QLabel* m_Icon;
    QLabel* m_Description;
    QLabel* m_liftTime;
    QPushButton* m_PushButton;
    QHBoxLayout* m_HBoxLayout1;
    QHBoxLayout* m_HBoxLayout2;
    QVBoxLayout* m_VBoxLayout;
};

#endif // DLGINFORMATIONWINDOW_H
