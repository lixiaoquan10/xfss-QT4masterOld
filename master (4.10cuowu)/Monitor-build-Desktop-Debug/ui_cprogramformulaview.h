/********************************************************************************
** Form generated from reading UI file 'cprogramformulaview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPROGRAMFORMULAVIEW_H
#define UI_CPROGRAMFORMULAVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "style/cstyledbar.h"
#include "view/ctableview.h"
#include "widget/cminisplitter.h"

QT_BEGIN_NAMESPACE

class Ui_CProgramFormulaView
{
public:
    QVBoxLayout *verticalLayout_3;
    CMiniSplitter *splitter;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    CStyledBar *m_inToolBar;
    CTableView *m_inTableView;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    CStyledBar *m_outToolBar;
    CTableView *m_outTableView;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *m_saveButton;
    QToolButton *m_cancelButton;

    void setupUi(QWidget *CProgramFormulaView)
    {
        if (CProgramFormulaView->objectName().isEmpty())
            CProgramFormulaView->setObjectName(QString::fromUtf8("CProgramFormulaView"));
        CProgramFormulaView->resize(817, 513);
        verticalLayout_3 = new QVBoxLayout(CProgramFormulaView);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 6);
        splitter = new CMiniSplitter(CProgramFormulaView);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_inToolBar = new CStyledBar(verticalLayoutWidget);
        m_inToolBar->setObjectName(QString::fromUtf8("m_inToolBar"));

        verticalLayout->addWidget(m_inToolBar);

        m_inTableView = new CTableView(verticalLayoutWidget);
        m_inTableView->setObjectName(QString::fromUtf8("m_inTableView"));

        verticalLayout->addWidget(m_inTableView);

        splitter->addWidget(verticalLayoutWidget);
        verticalLayoutWidget_2 = new QWidget(splitter);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        m_outToolBar = new CStyledBar(verticalLayoutWidget_2);
        m_outToolBar->setObjectName(QString::fromUtf8("m_outToolBar"));

        verticalLayout_2->addWidget(m_outToolBar);

        m_outTableView = new CTableView(verticalLayoutWidget_2);
        m_outTableView->setObjectName(QString::fromUtf8("m_outTableView"));

        verticalLayout_2->addWidget(m_outTableView);

        splitter->addWidget(verticalLayoutWidget_2);

        verticalLayout_3->addWidget(splitter);

        groupBox = new QGroupBox(CProgramFormulaView);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(9);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 10, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_saveButton = new QToolButton(CProgramFormulaView);
        m_saveButton->setObjectName(QString::fromUtf8("m_saveButton"));
        sizePolicy.setHeightForWidth(m_saveButton->sizePolicy().hasHeightForWidth());
        m_saveButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(m_saveButton);

        m_cancelButton = new QToolButton(CProgramFormulaView);
        m_cancelButton->setObjectName(QString::fromUtf8("m_cancelButton"));
        sizePolicy.setHeightForWidth(m_cancelButton->sizePolicy().hasHeightForWidth());
        m_cancelButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(m_cancelButton);

        horizontalLayout->setStretch(0, 6);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);

        verticalLayout_3->addLayout(horizontalLayout);

        verticalLayout_3->setStretch(0, 12);
        verticalLayout_3->setStretch(2, 1);

        retranslateUi(CProgramFormulaView);

        QMetaObject::connectSlotsByName(CProgramFormulaView);
    } // setupUi

    void retranslateUi(QWidget *CProgramFormulaView)
    {
        CProgramFormulaView->setWindowTitle(QApplication::translate("CProgramFormulaView", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        m_saveButton->setText(QApplication::translate("CProgramFormulaView", "save", 0, QApplication::UnicodeUTF8));
        m_cancelButton->setText(QApplication::translate("CProgramFormulaView", "cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CProgramFormulaView: public Ui_CProgramFormulaView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPROGRAMFORMULAVIEW_H
