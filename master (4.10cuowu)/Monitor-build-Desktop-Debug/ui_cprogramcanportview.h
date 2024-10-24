/********************************************************************************
** Form generated from reading UI file 'cprogramcanportview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPROGRAMCANPORTVIEW_H
#define UI_CPROGRAMCANPORTVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "style/cstyledbar.h"

QT_BEGIN_NAMESPACE

class Ui_CProgramCanportView
{
public:
    QVBoxLayout *verticalLayout;
    CStyledBar *m_pToolBar;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditId;
    QLabel *label_4;
    QLineEdit *lineEditName;
    QPushButton *pushButtonSet;
    QTableView *tableView;

    void setupUi(QWidget *CProgramCanportView)
    {
        if (CProgramCanportView->objectName().isEmpty())
            CProgramCanportView->setObjectName(QString::fromUtf8("CProgramCanportView"));
        CProgramCanportView->resize(829, 361);
        CProgramCanportView->setWindowTitle(QString::fromUtf8("ControllerView"));
        verticalLayout = new QVBoxLayout(CProgramCanportView);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_pToolBar = new CStyledBar(CProgramCanportView);
        m_pToolBar->setObjectName(QString::fromUtf8("m_pToolBar"));

        verticalLayout->addWidget(m_pToolBar);

        groupBox = new QGroupBox(CProgramCanportView);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditId = new QLineEdit(groupBox);
        lineEditId->setObjectName(QString::fromUtf8("lineEditId"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditId->sizePolicy().hasHeightForWidth());
        lineEditId->setSizePolicy(sizePolicy);
        lineEditId->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(lineEditId);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(label_4);

        lineEditName = new QLineEdit(groupBox);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEditName->sizePolicy().hasHeightForWidth());
        lineEditName->setSizePolicy(sizePolicy2);
        lineEditName->setMinimumSize(QSize(0, 0));
        lineEditName->setCursorPosition(0);

        horizontalLayout->addWidget(lineEditName);

        pushButtonSet = new QPushButton(groupBox);
        pushButtonSet->setObjectName(QString::fromUtf8("pushButtonSet"));
        sizePolicy1.setHeightForWidth(pushButtonSet->sizePolicy().hasHeightForWidth());
        pushButtonSet->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButtonSet);


        verticalLayout->addWidget(groupBox);

        tableView = new QTableView(CProgramCanportView);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setPointSize(12);
        tableView->setFont(font);
        tableView->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(tableView);


        retranslateUi(CProgramCanportView);

        QMetaObject::connectSlotsByName(CProgramCanportView);
    } // setupUi

    void retranslateUi(QWidget *CProgramCanportView)
    {
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("CProgramCanportView", "Controller Id:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CProgramCanportView", "Controller Name:", 0, QApplication::UnicodeUTF8));
        pushButtonSet->setText(QApplication::translate("CProgramCanportView", "Set", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CProgramCanportView);
    } // retranslateUi

};

namespace Ui {
    class CProgramCanportView: public Ui_CProgramCanportView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPROGRAMCANPORTVIEW_H
