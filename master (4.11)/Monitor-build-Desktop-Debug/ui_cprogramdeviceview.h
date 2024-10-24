/********************************************************************************
** Form generated from reading UI file 'cprogramdeviceview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPROGRAMDEVICEVIEW_H
#define UI_CPROGRAMDEVICEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <view/ctableview.h>
#include "style/cstyledbar.h"

QT_BEGIN_NAMESPACE

class Ui_CProgramDeviceView
{
public:
    QVBoxLayout *verticalLayout;
    CStyledBar *m_pToolBar;
    CTableView *tableView;

    void setupUi(QWidget *CProgramDeviceView)
    {
        if (CProgramDeviceView->objectName().isEmpty())
            CProgramDeviceView->setObjectName(QString::fromUtf8("CProgramDeviceView"));
        CProgramDeviceView->resize(592, 389);
        CProgramDeviceView->setWindowTitle(QString::fromUtf8("Form"));
        verticalLayout = new QVBoxLayout(CProgramDeviceView);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_pToolBar = new CStyledBar(CProgramDeviceView);
        m_pToolBar->setObjectName(QString::fromUtf8("m_pToolBar"));

        verticalLayout->addWidget(m_pToolBar);

        tableView = new CTableView(CProgramDeviceView);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        QFont font;
        font.setPointSize(12);
        tableView->setFont(font);
        tableView->setStyleSheet(QString::fromUtf8(""));

        verticalLayout->addWidget(tableView);


        retranslateUi(CProgramDeviceView);

        QMetaObject::connectSlotsByName(CProgramDeviceView);
    } // setupUi

    void retranslateUi(QWidget *CProgramDeviceView)
    {
        Q_UNUSED(CProgramDeviceView);
    } // retranslateUi

};

namespace Ui {
    class CProgramDeviceView: public Ui_CProgramDeviceView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPROGRAMDEVICEVIEW_H
