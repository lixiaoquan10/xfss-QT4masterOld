#ifndef DLGEDITTEXTITEM_H
#define DLGEDITTEXTITEM_H

#include <QDialog>
#include <QColor>
#include <QString>

namespace Ui {
class dlgEditTextItem;
}

class dlgEditTextItem : public QDialog
{
    Q_OBJECT
    
public:
    explicit dlgEditTextItem(QWidget *parent = 0);
    ~dlgEditTextItem();

    QColor color() { return m_cColor; }
    void setColor(const QColor& color);

    QString text() { return m_sText; }
    void setText(const QString& str);

    int size() { return m_iSize; }
    void setSize(const int n);
private slots:
    void slotSetSize(int n);
    void on_pushButtonColor_clicked();
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::dlgEditTextItem *ui;
    QColor m_cColor;
    QString m_sText;
    int m_iSize;
};

#endif // DLGEDITTEXTITEM_H
