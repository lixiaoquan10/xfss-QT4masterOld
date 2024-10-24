/********************************************************************************
** Form generated from reading UI file 'dlguploadlamp.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGUPLOADLAMP_H
#define UI_DLGUPLOADLAMP_H

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

class Ui_dlgUploadLamp
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *pushButton;

    void setupUi(QDialog *dlgUploadLamp)
    {
        if (dlgUploadLamp->objectName().isEmpty())
            dlgUploadLamp->setObjectName(QString::fromUtf8("dlgUploadLamp"));
        dlgUploadLamp->resize(324, 47);
        QFont font;
        font.setPointSize(12);
        dlgUploadLamp->setFont(font);
        horizontalLayout = new QHBoxLayout(dlgUploadLamp);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        label = new QLabel(dlgUploadLamp);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(dlgUploadLamp);
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

        pushButton = new QPushButton(dlgUploadLamp);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(pushButton);


        retranslateUi(dlgUploadLamp);

        QMetaObject::connectSlotsByName(dlgUploadLamp);
    } // setupUi

    void retranslateUi(QDialog *dlgUploadLamp)
    {
        dlgUploadLamp->setWindowTitle(QApplication::translate("dlgUploadLamp", "\346\233\277\346\215\242\346\227\247\347\201\257\345\205\267", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgUploadLamp", "\346\233\277\346\215\242\346\227\247\347\201\257\345\205\267\345\234\260\345\235\200:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("dlgUploadLamp", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgUploadLamp: public Ui_dlgUploadLamp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGUPLOADLAMP_H
