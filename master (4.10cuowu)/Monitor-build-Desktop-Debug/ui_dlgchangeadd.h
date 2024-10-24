/********************************************************************************
** Form generated from reading UI file 'dlgchangeadd.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCHANGEADD_H
#define UI_DLGCHANGEADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_dlgChangeAdd
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *pushButton;

    void setupUi(QDialog *dlgChangeAdd)
    {
        if (dlgChangeAdd->objectName().isEmpty())
            dlgChangeAdd->setObjectName(QString::fromUtf8("dlgChangeAdd"));
        dlgChangeAdd->resize(282, 47);
        QFont font;
        font.setPointSize(12);
        dlgChangeAdd->setFont(font);
        dlgChangeAdd->setStyleSheet(QString::fromUtf8("border: 1px solid black; "));
        horizontalLayout = new QHBoxLayout(dlgChangeAdd);
#ifndef Q_OS_MAC
        horizontalLayout->setSpacing(6);
#endif
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(dlgChangeAdd);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("border: none;"));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(dlgChangeAdd);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setPointSize(14);
        comboBox->setFont(font1);

        horizontalLayout->addWidget(comboBox);

        pushButton = new QPushButton(dlgChangeAdd);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);
        pushButton->setMinimumSize(QSize(50, 0));
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8(" background-color: rgb(84, 100, 245);border: none;"));

        horizontalLayout->addWidget(pushButton);


        retranslateUi(dlgChangeAdd);

        QMetaObject::connectSlotsByName(dlgChangeAdd);
    } // setupUi

    void retranslateUi(QDialog *dlgChangeAdd)
    {
        dlgChangeAdd->setWindowTitle(QApplication::translate("dlgChangeAdd", "\344\277\256\346\224\271\347\201\257\345\205\267\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgChangeAdd", "\350\256\276\345\244\207\345\234\260\345\235\200:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("dlgChangeAdd", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgChangeAdd: public Ui_dlgChangeAdd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCHANGEADD_H
