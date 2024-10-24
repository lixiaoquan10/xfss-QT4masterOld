/********************************************************************************
** Form generated from reading UI file 'dlghandfireview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGHANDFIREVIEW_H
#define UI_DLGHANDFIREVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dlgHandFireView
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_fire;
    QPushButton *pushButton_close;

    void setupUi(QDialog *dlgHandFireView)
    {
        if (dlgHandFireView->objectName().isEmpty())
            dlgHandFireView->setObjectName(QString::fromUtf8("dlgHandFireView"));
        dlgHandFireView->resize(722, 403);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dlgHandFireView->sizePolicy().hasHeightForWidth());
        dlgHandFireView->setSizePolicy(sizePolicy);
        dlgHandFireView->setMinimumSize(QSize(0, 0));
        dlgHandFireView->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(12);
        dlgHandFireView->setFont(font);
        dlgHandFireView->setInputMethodHints(Qt::ImhNone);
        verticalLayout = new QVBoxLayout(dlgHandFireView);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeWidget = new QTreeWidget(dlgHandFireView);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(0, font);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setFont(font);

        verticalLayout->addWidget(treeWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label = new QLabel(dlgHandFireView);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_fire = new QPushButton(dlgHandFireView);
        pushButton_fire->setObjectName(QString::fromUtf8("pushButton_fire"));
        pushButton_fire->setMinimumSize(QSize(80, 35));
        pushButton_fire->setFont(font);
        pushButton_fire->setCheckable(false);

        horizontalLayout->addWidget(pushButton_fire);

        pushButton_close = new QPushButton(dlgHandFireView);
        pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));
        pushButton_close->setMinimumSize(QSize(60, 35));

        horizontalLayout->addWidget(pushButton_close);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(dlgHandFireView);

        QMetaObject::connectSlotsByName(dlgHandFireView);
    } // setupUi

    void retranslateUi(QDialog *dlgHandFireView)
    {
        dlgHandFireView->setWindowTitle(QApplication::translate("dlgHandFireView", "Dialog", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("dlgHandFireView", "\347\201\253\350\255\246\347\202\271", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgHandFireView", "\351\200\211\344\270\255\347\201\253\350\255\246\347\202\271\345\220\216\347\202\271\345\207\273\346\250\241\346\213\237\347\201\253\350\255\246!!!", 0, QApplication::UnicodeUTF8));
        pushButton_fire->setText(QApplication::translate("dlgHandFireView", "\346\250\241\346\213\237\347\201\253\350\255\246", 0, QApplication::UnicodeUTF8));
        pushButton_close->setText(QApplication::translate("dlgHandFireView", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgHandFireView: public Ui_dlgHandFireView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGHANDFIREVIEW_H
