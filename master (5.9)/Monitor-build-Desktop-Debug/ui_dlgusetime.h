/********************************************************************************
** Form generated from reading UI file 'dlgusetime.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGUSETIME_H
#define UI_DLGUSETIME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dlgUseTime
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_message;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *m_linkmanPhoneNumber;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *m_LocalNumber;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *m_rPowerCode;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *m_btnCancel;
    QPushButton *m_btnOk;

    void setupUi(QDialog *dlgUseTime)
    {
        if (dlgUseTime->objectName().isEmpty())
            dlgUseTime->setObjectName(QString::fromUtf8("dlgUseTime"));
        dlgUseTime->resize(280, 230);
        dlgUseTime->setMaximumSize(QSize(280, 230));
        dlgUseTime->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 255, 255);"));
        verticalLayout = new QVBoxLayout(dlgUseTime);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_message = new QLabel(dlgUseTime);
        label_message->setObjectName(QString::fromUtf8("label_message"));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu"));
        font.setPointSize(11);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        label_message->setFont(font);

        verticalLayout->addWidget(label_message);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(dlgUseTime);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        m_linkmanPhoneNumber = new QLineEdit(dlgUseTime);
        m_linkmanPhoneNumber->setObjectName(QString::fromUtf8("m_linkmanPhoneNumber"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_linkmanPhoneNumber->sizePolicy().hasHeightForWidth());
        m_linkmanPhoneNumber->setSizePolicy(sizePolicy);
        m_linkmanPhoneNumber->setMinimumSize(QSize(0, 28));
        m_linkmanPhoneNumber->setMaximumSize(QSize(16777215, 28));
        m_linkmanPhoneNumber->setEchoMode(QLineEdit::Normal);
        m_linkmanPhoneNumber->setReadOnly(false);

        horizontalLayout_4->addWidget(m_linkmanPhoneNumber);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(dlgUseTime);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        m_LocalNumber = new QLineEdit(dlgUseTime);
        m_LocalNumber->setObjectName(QString::fromUtf8("m_LocalNumber"));
        sizePolicy.setHeightForWidth(m_LocalNumber->sizePolicy().hasHeightForWidth());
        m_LocalNumber->setSizePolicy(sizePolicy);
        m_LocalNumber->setMinimumSize(QSize(0, 28));
        m_LocalNumber->setMaximumSize(QSize(16777215, 28));
        m_LocalNumber->setEchoMode(QLineEdit::Normal);
        m_LocalNumber->setReadOnly(true);

        horizontalLayout->addWidget(m_LocalNumber);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(dlgUseTime);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        m_rPowerCode = new QLineEdit(dlgUseTime);
        m_rPowerCode->setObjectName(QString::fromUtf8("m_rPowerCode"));
        sizePolicy.setHeightForWidth(m_rPowerCode->sizePolicy().hasHeightForWidth());
        m_rPowerCode->setSizePolicy(sizePolicy);
        m_rPowerCode->setMinimumSize(QSize(0, 28));
        m_rPowerCode->setMaximumSize(QSize(16777215, 28));
        m_rPowerCode->setEchoMode(QLineEdit::Normal);

        horizontalLayout_2->addWidget(m_rPowerCode);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        m_btnCancel = new QPushButton(dlgUseTime);
        m_btnCancel->setObjectName(QString::fromUtf8("m_btnCancel"));
        m_btnCancel->setMinimumSize(QSize(0, 25));

        horizontalLayout_3->addWidget(m_btnCancel);

        m_btnOk = new QPushButton(dlgUseTime);
        m_btnOk->setObjectName(QString::fromUtf8("m_btnOk"));
        m_btnOk->setMinimumSize(QSize(0, 25));

        horizontalLayout_3->addWidget(m_btnOk);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(dlgUseTime);

        QMetaObject::connectSlotsByName(dlgUseTime);
    } // setupUi

    void retranslateUi(QDialog *dlgUseTime)
    {
        dlgUseTime->setWindowTitle(QApplication::translate("dlgUseTime", "\344\275\277\347\224\250\346\234\237\351\231\220", 0, QApplication::UnicodeUTF8));
        label_message->setText(QApplication::translate("dlgUseTime", "\350\257\267\350\276\223\345\205\245\346\235\203\351\231\220\345\272\217\345\217\267", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("dlgUseTime", "\350\201\224\347\263\273\347\224\265\350\257\235:", 0, QApplication::UnicodeUTF8));
        m_linkmanPhoneNumber->setPlaceholderText(QString());
        label_2->setText(QApplication::translate("dlgUseTime", "\346\234\254\346\234\272\345\272\217\345\217\267:", 0, QApplication::UnicodeUTF8));
        m_LocalNumber->setPlaceholderText(QString());
        label->setText(QApplication::translate("dlgUseTime", "\346\235\203\351\231\220\345\272\217\345\217\267:", 0, QApplication::UnicodeUTF8));
        m_rPowerCode->setPlaceholderText(QString());
        m_btnCancel->setText(QApplication::translate("dlgUseTime", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        m_btnOk->setText(QApplication::translate("dlgUseTime", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgUseTime: public Ui_dlgUseTime {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGUSETIME_H
