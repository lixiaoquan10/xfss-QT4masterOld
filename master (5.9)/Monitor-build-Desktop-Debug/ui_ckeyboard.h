/********************************************************************************
** Form generated from reading UI file 'ckeyboard.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CKEYBOARD_H
#define UI_CKEYBOARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CKeyboard
{
public:
    QPushButton *pushButton_delete;
    QPushButton *pushButton_close;
    QWidget *candidateArea;
    QHBoxLayout *horizontalLayout;
    QLineEdit *LineEdit_Echo;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_dian;
    QPushButton *pushButton_0;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QDialog *CKeyboard)
    {
        if (CKeyboard->objectName().isEmpty())
            CKeyboard->setObjectName(QString::fromUtf8("CKeyboard"));
        CKeyboard->resize(200, 241);
        CKeyboard->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton_delete = new QPushButton(CKeyboard);
        pushButton_delete->setObjectName(QString::fromUtf8("pushButton_delete"));
        pushButton_delete->setGeometry(QRect(20, 200, 75, 36));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_delete->sizePolicy().hasHeightForWidth());
        pushButton_delete->setSizePolicy(sizePolicy);
        pushButton_delete->setMinimumSize(QSize(36, 36));
        QFont font;
        font.setPointSize(17);
        pushButton_delete->setFont(font);
        pushButton_delete->setStyleSheet(QString::fromUtf8("background-color: rgb(65, 105, 225);"));
        pushButton_close = new QPushButton(CKeyboard);
        pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));
        pushButton_close->setGeometry(QRect(105, 200, 75, 36));
        sizePolicy.setHeightForWidth(pushButton_close->sizePolicy().hasHeightForWidth());
        pushButton_close->setSizePolicy(sizePolicy);
        pushButton_close->setMinimumSize(QSize(36, 36));
        pushButton_close->setFont(font);
        pushButton_close->setStyleSheet(QString::fromUtf8("background-color: rgb(65, 105, 225);"));
        candidateArea = new QWidget(CKeyboard);
        candidateArea->setObjectName(QString::fromUtf8("candidateArea"));
        candidateArea->setGeometry(QRect(10, 10, 158, 21));
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(false);
        font1.setWeight(50);
        candidateArea->setFont(font1);
        horizontalLayout = new QHBoxLayout(candidateArea);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 0, 0, 0);
        LineEdit_Echo = new QLineEdit(candidateArea);
        LineEdit_Echo->setObjectName(QString::fromUtf8("LineEdit_Echo"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(LineEdit_Echo->sizePolicy().hasHeightForWidth());
        LineEdit_Echo->setSizePolicy(sizePolicy1);
        LineEdit_Echo->setFrame(false);

        horizontalLayout->addWidget(LineEdit_Echo);

        pushButton_7 = new QPushButton(CKeyboard);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(20, 120, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy);
        pushButton_7->setMinimumSize(QSize(36, 36));
        pushButton_7->setFont(font);
        pushButton_8 = new QPushButton(CKeyboard);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(75, 120, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy);
        pushButton_8->setMinimumSize(QSize(36, 36));
        pushButton_8->setFont(font);
        pushButton_9 = new QPushButton(CKeyboard);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(130, 120, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy);
        pushButton_9->setMinimumSize(QSize(36, 36));
        pushButton_9->setFont(font);
        pushButton_dian = new QPushButton(CKeyboard);
        pushButton_dian->setObjectName(QString::fromUtf8("pushButton_dian"));
        pushButton_dian->setGeometry(QRect(20, 160, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_dian->sizePolicy().hasHeightForWidth());
        pushButton_dian->setSizePolicy(sizePolicy);
        pushButton_dian->setMinimumSize(QSize(36, 36));
        pushButton_dian->setFont(font);
        pushButton_0 = new QPushButton(CKeyboard);
        pushButton_0->setObjectName(QString::fromUtf8("pushButton_0"));
        pushButton_0->setGeometry(QRect(75, 160, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_0->sizePolicy().hasHeightForWidth());
        pushButton_0->setSizePolicy(sizePolicy);
        pushButton_0->setMinimumSize(QSize(36, 36));
        pushButton_0->setFont(font);
        pushButton_4 = new QPushButton(CKeyboard);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(20, 80, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(36, 36));
        pushButton_4->setFont(font);
        pushButton_5 = new QPushButton(CKeyboard);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(75, 80, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy);
        pushButton_5->setMinimumSize(QSize(36, 36));
        pushButton_5->setFont(font);
        pushButton_6 = new QPushButton(CKeyboard);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(130, 80, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setMinimumSize(QSize(36, 36));
        pushButton_6->setFont(font);
        pushButton_1 = new QPushButton(CKeyboard);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(20, 40, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_1->sizePolicy().hasHeightForWidth());
        pushButton_1->setSizePolicy(sizePolicy);
        pushButton_1->setMinimumSize(QSize(36, 36));
        pushButton_1->setFont(font);
        pushButton_2 = new QPushButton(CKeyboard);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(75, 40, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(36, 36));
        pushButton_2->setFont(font);
        pushButton_3 = new QPushButton(CKeyboard);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(130, 40, 50, 36));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setMinimumSize(QSize(36, 36));
        pushButton_3->setFont(font);
        pushButton_delete->raise();
        pushButton_close->raise();
        candidateArea->raise();
        LineEdit_Echo->raise();
        pushButton_7->raise();
        pushButton_8->raise();
        pushButton_9->raise();
        pushButton_dian->raise();
        pushButton_0->raise();
        pushButton_4->raise();
        pushButton_5->raise();
        pushButton_6->raise();
        pushButton_1->raise();
        pushButton_2->raise();
        pushButton_3->raise();

        retranslateUi(CKeyboard);

        QMetaObject::connectSlotsByName(CKeyboard);
    } // setupUi

    void retranslateUi(QDialog *CKeyboard)
    {
        CKeyboard->setWindowTitle(QApplication::translate("CKeyboard", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton_delete->setText(QApplication::translate("CKeyboard", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        pushButton_close->setText(QApplication::translate("CKeyboard", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("CKeyboard", "7", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("CKeyboard", "8", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("CKeyboard", "9", 0, QApplication::UnicodeUTF8));
        pushButton_dian->setText(QApplication::translate("CKeyboard", ".", 0, QApplication::UnicodeUTF8));
        pushButton_0->setText(QApplication::translate("CKeyboard", "0", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("CKeyboard", "4", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("CKeyboard", "5", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("CKeyboard", "6", 0, QApplication::UnicodeUTF8));
        pushButton_1->setText(QApplication::translate("CKeyboard", "1", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("CKeyboard", "2", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("CKeyboard", "3", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CKeyboard: public Ui_CKeyboard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CKEYBOARD_H
