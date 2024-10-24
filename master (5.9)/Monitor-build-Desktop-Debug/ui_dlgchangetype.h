/********************************************************************************
** Form generated from reading UI file 'dlgchangetype.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCHANGETYPE_H
#define UI_DLGCHANGETYPE_H

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

class Ui_dlgChangeType
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *dlgChangeType)
    {
        if (dlgChangeType->objectName().isEmpty())
            dlgChangeType->setObjectName(QString::fromUtf8("dlgChangeType"));
        dlgChangeType->resize(368, 50);
        dlgChangeType->setMinimumSize(QSize(0, 50));
        dlgChangeType->setMaximumSize(QSize(16777215, 50));
        QFont font;
        font.setPointSize(12);
        dlgChangeType->setFont(font);
        dlgChangeType->setStyleSheet(QString::fromUtf8("border: 1px solid black;"));
        horizontalLayout = new QHBoxLayout(dlgChangeType);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(dlgChangeType);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 30));
        label->setMaximumSize(QSize(16777215, 30));
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("border: none;"));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(dlgChangeType);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        comboBox->setMinimumSize(QSize(0, 30));
        comboBox->setMaximumSize(QSize(16777215, 30));
        QFont font1;
        font1.setPointSize(14);
        comboBox->setFont(font1);

        horizontalLayout->addWidget(comboBox);

        pushButton = new QPushButton(dlgChangeType);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);
        pushButton->setMinimumSize(QSize(50, 30));
        pushButton->setMaximumSize(QSize(16777215, 30));
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));

        horizontalLayout->addWidget(pushButton);

        pushButton_cancel = new QPushButton(dlgChangeType);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));
        sizePolicy2.setHeightForWidth(pushButton_cancel->sizePolicy().hasHeightForWidth());
        pushButton_cancel->setSizePolicy(sizePolicy2);
        pushButton_cancel->setMinimumSize(QSize(50, 30));
        pushButton_cancel->setMaximumSize(QSize(16777215, 30));
        pushButton_cancel->setFont(font);
        pushButton_cancel->setStyleSheet(QString::fromUtf8("background-color: rgb(84, 100, 245);border: none;"));

        horizontalLayout->addWidget(pushButton_cancel);


        retranslateUi(dlgChangeType);

        QMetaObject::connectSlotsByName(dlgChangeType);
    } // setupUi

    void retranslateUi(QDialog *dlgChangeType)
    {
        dlgChangeType->setWindowTitle(QApplication::translate("dlgChangeType", "\344\277\256\346\224\271\350\256\276\345\244\207\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgChangeType", "\350\256\276\345\244\207\347\261\273\345\236\213:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("dlgChangeType", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        pushButton_cancel->setText(QApplication::translate("dlgChangeType", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgChangeType: public Ui_dlgChangeType {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCHANGETYPE_H
