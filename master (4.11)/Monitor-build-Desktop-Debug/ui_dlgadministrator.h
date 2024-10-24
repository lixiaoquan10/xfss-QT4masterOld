/********************************************************************************
** Form generated from reading UI file 'dlgadministrator.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGADMINISTRATOR_H
#define UI_DLGADMINISTRATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_dlgAdministrator
{
public:
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_6;
    QPushButton *pushButton_5;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QPushButton *pushButton_8;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_empty;
    QPushButton *pushButton_0;
    QLabel *label;
    QLineEdit *m_PassWord;
    QLabel *label_2;
    QComboBox *comboBox;

    void setupUi(QDialog *dlgAdministrator)
    {
        if (dlgAdministrator->objectName().isEmpty())
            dlgAdministrator->setObjectName(QString::fromUtf8("dlgAdministrator"));
        dlgAdministrator->resize(295, 350);
        dlgAdministrator->setMaximumSize(QSize(300, 350));
        QFont font;
        font.setPointSize(15);
        dlgAdministrator->setFont(font);
        dlgAdministrator->setStyleSheet(QString::fromUtf8("border: 1px solid black;"));
        pushButton_1 = new QPushButton(dlgAdministrator);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(45, 105, 65, 50));
        QFont font1;
        font1.setPointSize(15);
        font1.setBold(true);
        font1.setWeight(75);
        pushButton_1->setFont(font1);
        pushButton_1->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_2 = new QPushButton(dlgAdministrator);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(115, 105, 65, 50));
        pushButton_2->setFont(font1);
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_3 = new QPushButton(dlgAdministrator);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(185, 105, 65, 50));
        pushButton_3->setFont(font1);
        pushButton_3->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_4 = new QPushButton(dlgAdministrator);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(45, 160, 65, 50));
        pushButton_4->setFont(font1);
        pushButton_4->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_6 = new QPushButton(dlgAdministrator);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(185, 160, 65, 50));
        pushButton_6->setFont(font1);
        pushButton_6->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_5 = new QPushButton(dlgAdministrator);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(115, 160, 65, 50));
        pushButton_5->setFont(font1);
        pushButton_5->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_7 = new QPushButton(dlgAdministrator);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(45, 215, 65, 50));
        pushButton_7->setFont(font1);
        pushButton_7->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_9 = new QPushButton(dlgAdministrator);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(185, 215, 65, 50));
        pushButton_9->setFont(font1);
        pushButton_9->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_8 = new QPushButton(dlgAdministrator);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(115, 215, 65, 50));
        pushButton_8->setFont(font1);
        pushButton_8->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_cancel = new QPushButton(dlgAdministrator);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));
        pushButton_cancel->setGeometry(QRect(45, 270, 65, 50));
        pushButton_cancel->setFont(font1);
        pushButton_cancel->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_empty = new QPushButton(dlgAdministrator);
        pushButton_empty->setObjectName(QString::fromUtf8("pushButton_empty"));
        pushButton_empty->setGeometry(QRect(185, 270, 65, 50));
        pushButton_empty->setFont(font1);
        pushButton_empty->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        pushButton_0 = new QPushButton(dlgAdministrator);
        pushButton_0->setObjectName(QString::fromUtf8("pushButton_0"));
        pushButton_0->setGeometry(QRect(115, 270, 65, 50));
        pushButton_0->setFont(font1);
        pushButton_0->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));
        label = new QLabel(dlgAdministrator);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(15, 60, 45, 30));
        QFont font2;
        font2.setPointSize(12);
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("border: none;"));
        m_PassWord = new QLineEdit(dlgAdministrator);
        m_PassWord->setObjectName(QString::fromUtf8("m_PassWord"));
        m_PassWord->setGeometry(QRect(80, 60, 130, 30));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_PassWord->sizePolicy().hasHeightForWidth());
        m_PassWord->setSizePolicy(sizePolicy);
        m_PassWord->setMinimumSize(QSize(0, 30));
        m_PassWord->setMaximumSize(QSize(16777215, 25));
        m_PassWord->setStyleSheet(QString::fromUtf8("border: 1px solid black;\n"
"background-color: rgb(160, 169, 178);"));
        m_PassWord->setEchoMode(QLineEdit::Normal);
        m_PassWord->setReadOnly(true);
        label_2 = new QLabel(dlgAdministrator);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(15, 20, 45, 30));
        label_2->setFont(font2);
        label_2->setStyleSheet(QString::fromUtf8("border: none;"));
        comboBox = new QComboBox(dlgAdministrator);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(80, 20, 130, 32));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        comboBox->setFont(font);
        comboBox->setStyleSheet(QString::fromUtf8("border: 1px solid black;"));

        retranslateUi(dlgAdministrator);

        QMetaObject::connectSlotsByName(dlgAdministrator);
    } // setupUi

    void retranslateUi(QDialog *dlgAdministrator)
    {
        dlgAdministrator->setWindowTitle(QApplication::translate("dlgAdministrator", "\347\224\250\346\210\267\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        pushButton_1->setText(QApplication::translate("dlgAdministrator", "1", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("dlgAdministrator", "2", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("dlgAdministrator", "3", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("dlgAdministrator", "4", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("dlgAdministrator", "6", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("dlgAdministrator", "5", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("dlgAdministrator", "7", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("dlgAdministrator", "9", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("dlgAdministrator", "8", 0, QApplication::UnicodeUTF8));
        pushButton_cancel->setText(QApplication::translate("dlgAdministrator", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        pushButton_empty->setText(QApplication::translate("dlgAdministrator", "\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
        pushButton_0->setText(QApplication::translate("dlgAdministrator", "0", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgAdministrator", "\345\257\206\347\240\201:", 0, QApplication::UnicodeUTF8));
        m_PassWord->setPlaceholderText(QString());
        label_2->setText(QApplication::translate("dlgAdministrator", "\347\224\250\346\210\267:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgAdministrator: public Ui_dlgAdministrator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGADMINISTRATOR_H
