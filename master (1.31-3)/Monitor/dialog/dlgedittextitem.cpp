#include "dlgedittextitem.h"
#include "ui_dlgedittextitem.h"

#include <QMessageBox>
#include <QColorDialog>

dlgEditTextItem::dlgEditTextItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgEditTextItem),m_cColor(0, 0, 0)
{
    ui->setupUi(this);
    QPalette pal = ui->lineEditColor->palette();
    pal.setColor(QPalette::Base, m_cColor);
    ui->lineEditColor->setPalette(pal);
}

dlgEditTextItem::~dlgEditTextItem()
{
    delete ui;
}

void dlgEditTextItem::setColor(const QColor &color)
{
    m_cColor = color;
    QPalette pal = ui->lineEditColor->palette();
    pal.setColor(QPalette::Base, m_cColor);
    ui->lineEditColor->setPalette(pal);
}

void dlgEditTextItem::setText(const QString &str)
{
     m_sText = str;
     ui->lineEditText->setText(m_sText);
}

void dlgEditTextItem::setSize(const int n)
{
    m_iSize = n;
    ui->spinBoxSize->setValue(m_iSize);
}

void dlgEditTextItem::slotSetSize(int n)
{
    m_iSize = n;
    ui->spinBoxSize->setValue(m_iSize);
}

void dlgEditTextItem::on_pushButtonColor_clicked()
{
    QColor color = QColorDialog::getColor(m_cColor, this, tr("text color"));
    if (color.isValid()) {
        m_cColor = color;
        QPalette pal = ui->lineEditColor->palette();
        pal.setColor(QPalette::Base, m_cColor);
        ui->lineEditColor->setPalette(pal);
    }
}

void dlgEditTextItem::on_pushButtonOk_clicked()
{
    if (ui->lineEditText->text().isEmpty())
    {
        QMessageBox::information(this, tr("Information"), tr("Please input a text"), QMessageBox::Ok);
        return;
    }
    m_sText = ui->lineEditText->text();

    QDialog::accept();
}

void dlgEditTextItem::on_pushButtonCancel_clicked()
{
    QDialog::reject();
}
