/********************************************************************************
** Form generated from reading UI file 'dlgsearchdevice.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSEARCHDEVICE_H
#define UI_DLGSEARCHDEVICE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dlgSearchDevice
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_2;
    QComboBox *comboBox_2;
    QTableView *tableView;

    void setupUi(QDialog *dlgSearchDevice)
    {
        if (dlgSearchDevice->objectName().isEmpty())
            dlgSearchDevice->setObjectName(QString::fromUtf8("dlgSearchDevice"));
        dlgSearchDevice->resize(660, 397);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dlgSearchDevice->sizePolicy().hasHeightForWidth());
        dlgSearchDevice->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(dlgSearchDevice);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        groupBox = new QGroupBox(dlgSearchDevice);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(0, 25));

        horizontalLayout->addWidget(comboBox);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(80, 16777215));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        comboBox_2 = new QComboBox(groupBox);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setMinimumSize(QSize(0, 25));

        horizontalLayout->addWidget(comboBox_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        tableView = new QTableView(dlgSearchDevice);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);


        retranslateUi(dlgSearchDevice);

        QMetaObject::connectSlotsByName(dlgSearchDevice);
    } // setupUi

    void retranslateUi(QDialog *dlgSearchDevice)
    {
        dlgSearchDevice->setWindowTitle(QApplication::translate("dlgSearchDevice", "Search Device", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("dlgSearchDevice", "Query Condition", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgSearchDevice", "Distribution:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("dlgSearchDevice", "Loop:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgSearchDevice: public Ui_dlgSearchDevice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSEARCHDEVICE_H
