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
    QProgressBar* m_ProgressBar;
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    QMovie *movie;
    QLabel* m_Icon;
    QLabel* m_Description;
    QHBoxLayout* m_HBoxLayout;
    QVBoxLayout* m_VBoxLayout;
};

#endif // DLGINFORMATIONWINDOW_H
