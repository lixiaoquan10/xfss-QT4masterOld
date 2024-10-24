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
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *dlgChangeAdd)
    {
        if (dlgChangeAdd->objectName().isEmpty())
            dlgChangeAdd->setObjectName(QString::fromUtf8("dlgChangeAdd"));
        dlgChangeAdd->resize(310, 50);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dlgChangeAdd->sizePolicy().hasHeightForWidth());
        dlgChangeAdd->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(12);
        dlgChangeAdd->setFont(font);
        dlgChangeAdd->setStyleSheet(QString::fromUtf8("border: 1px solid black; "));
        horizontalLayout = new QHBoxLayout(dlgChangeAdd);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(dlgChangeAdd);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(0, 30));
        label->setMaximumSize(QSize(16777215, 30));
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("border: none;"));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(dlgChangeAdd);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy2);
        comboBox->setMinimumSize(QSize(0, 30));
        comboBox->setMaximumSize(QSize(16777215, 30));
        QFont font1;
        font1.setPointSize(14);
        comboBox->setFont(font1);

        horizontalLayout->addWidget(comboBox);

        pushButton = new QPushButton(dlgChangeAdd);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(50, 30));
        pushButton->setMaximumSize(QSize(16777215, 30));
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8(" background-color: rgb(84, 100, 245);border: none;"));

        horizontalLayout->addWidget(pushButton);

        pushButton_cancel = new QPushButton(dlgChangeAdd);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));
        sizePolicy.setHeightForWidth(pushButton_cancel->sizePolicy().hasHeightForWidth());
        pushButton_cancel->setSizePolicy(sizePolicy);
        pushButton_cancel->setMinimumSize(QSize(50, 30));
        pushButton_cancel->setMaximumSize(QSize(16777215, 30));
        QPalette palette;
        QBrush brush(QColor(84, 100, 245, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pushButton_cancel->setPalette(palette);
        pushButton_cancel->setFont(font);
        pushButton_cancel->setStyleSheet(QString::fromUtf8(" background-color: rgb(84, 100, 245);border: none;"));

        horizontalLayout->addWidget(pushButton_cancel);


        retranslateUi(dlgChangeAdd);

        QMetaObject::connectSlotsByName(dlgChangeAdd);
    } // setupUi

    void retranslateUi(QDialog *dlgChangeAdd)
    {
        dlgChangeAdd->setWindowTitle(QApplication::translate("dlgChangeAdd", "\344\277\256\346\224\271\347\201\257\345\205\267\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgChangeAdd", "\350\256\276\345\244\207\345\234\260\345\235\200:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("dlgChangeAdd", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        pushButton_cancel->setText(QApplication::translate("dlgChangeAdd", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgChangeAdd: public Ui_dlgChangeAdd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCHANGEADD_H
