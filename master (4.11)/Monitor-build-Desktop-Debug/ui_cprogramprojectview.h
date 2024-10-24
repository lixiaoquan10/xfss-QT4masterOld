/********************************************************************************
** Form generated from reading UI file 'cprogramprojectview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPROGRAMPROJECTVIEW_H
#define UI_CPROGRAMPROJECTVIEW_H

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
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <view/ctableview.h>
#include "style/cstyledbar.h"

QT_BEGIN_NAMESPACE

class Ui_CProgramControllerView
{
public:
    QVBoxLayout *verticalLayout;
    CStyledBar *m_pToolBar;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLineEdit *lineEditName;
    QLabel *label;
    QLineEdit *lineEditCodeRule;
    QPushButton *pushButtonSet;
    CTableView *tableView;

    void setupUi(QWidget *CProgramControllerView)
    {
        if (CProgramControllerView->objectName().isEmpty())
            CProgramControllerView->setObjectName(QString::fromUtf8("CProgramControllerView"));
        CProgramControllerView->resize(801, 438);
        CProgramControllerView->setWindowTitle(QString::fromUtf8("Form"));
        verticalLayout = new QVBoxLayout(CProgramControllerView);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_pToolBar = new CStyledBar(CProgramControllerView);
        m_pToolBar->setObjectName(QString::fromUtf8("m_pToolBar"));

        verticalLayout->addWidget(m_pToolBar);

        groupBox = new QGroupBox(CProgramControllerView);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        lineEditName = new QLineEdit(groupBox);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditName->sizePolicy().hasHeightForWidth());
        lineEditName->setSizePolicy(sizePolicy);
        lineEditName->setMinimumSize(QSize(0, 0));
        lineEditName->setCursorPosition(0);

        horizontalLayout->addWidget(lineEditName);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditCodeRule = new QLineEdit(groupBox);
        lineEditCodeRule->setObjectName(QString::fromUtf8("lineEditCodeRule"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEditCodeRule->sizePolicy().hasHeightForWidth());
        lineEditCodeRule->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(lineEditCodeRule);

        pushButtonSet = new QPushButton(groupBox);
        pushButtonSet->setObjectName(QString::fromUtf8("pushButtonSet"));

        horizontalLayout->addWidget(pushButtonSet);


        verticalLayout->addWidget(groupBox);

        tableView = new CTableView(CProgramControllerView);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        QFont font;
        font.setPointSize(12);
        tableView->setFont(font);
        tableView->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(tableView);


        retranslateUi(CProgramControllerView);

        QMetaObject::connectSlotsByName(CProgramControllerView);
    } // setupUi

    void retranslateUi(QWidget *CProgramControllerView)
    {
        groupBox->setTitle(QString());
        label_4->setText(QApplication::translate("CProgramControllerView", "Project Name:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CProgramControllerView", "Project Address:", 0, QApplication::UnicodeUTF8));
        pushButtonSet->setText(QApplication::translate("CProgramControllerView", "Set", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CProgramControllerView);
    } // retranslateUi

};

namespace Ui {
    class CProgramControllerView: public Ui_CProgramControllerView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPROGRAMPROJECTVIEW_H
