/********************************************************************************
** Form generated from reading UI file 'dlgsystemrecord.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSYSTEMRECORD_H
#define UI_DLGSYSTEMRECORD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dlgSystemRecord
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *m_WorklogType;
    QLabel *label_2;
    QDateEdit *m_begin_dateEdit;
    QLabel *label_3;
    QDateEdit *m_end_dateEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_before_query;
    QPushButton *pushButton_next_query;
    QLabel *label_page_query;
    QLineEdit *lineEdit_page_query;
    QSpacerItem *horizontalSpacer;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_page;
    QSpacerItem *horizontalSpacer_page;
    QLabel *label_page;
    QLineEdit *lineEdit_page;
    QPushButton *pushButton_before;
    QPushButton *pushButton_next;

    void setupUi(QDialog *dlgSystemRecord)
    {
        if (dlgSystemRecord->objectName().isEmpty())
            dlgSystemRecord->setObjectName(QString::fromUtf8("dlgSystemRecord"));
        dlgSystemRecord->resize(1045, 342);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dlgSystemRecord->sizePolicy().hasHeightForWidth());
        dlgSystemRecord->setSizePolicy(sizePolicy);
        dlgSystemRecord->setMinimumSize(QSize(0, 212));
        QFont font;
        font.setPointSize(12);
        dlgSystemRecord->setFont(font);
        verticalLayout_2 = new QVBoxLayout(dlgSystemRecord);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(dlgSystemRecord);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(45, 20));
        label->setFont(font);

        horizontalLayout->addWidget(label);

        m_WorklogType = new QComboBox(groupBox);
        m_WorklogType->setObjectName(QString::fromUtf8("m_WorklogType"));
        m_WorklogType->setMinimumSize(QSize(60, 25));
        QFont font1;
        font1.setPointSize(13);
        m_WorklogType->setFont(font1);

        horizontalLayout->addWidget(m_WorklogType);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(60, 20));
        label_2->setFont(font);

        horizontalLayout->addWidget(label_2);

        m_begin_dateEdit = new QDateEdit(groupBox);
        m_begin_dateEdit->setObjectName(QString::fromUtf8("m_begin_dateEdit"));
        m_begin_dateEdit->setMinimumSize(QSize(60, 25));
        m_begin_dateEdit->setFont(font1);
        m_begin_dateEdit->setCalendarPopup(true);

        horizontalLayout->addWidget(m_begin_dateEdit);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(60, 20));
        label_3->setFont(font);

        horizontalLayout->addWidget(label_3);

        m_end_dateEdit = new QDateEdit(groupBox);
        m_end_dateEdit->setObjectName(QString::fromUtf8("m_end_dateEdit"));
        m_end_dateEdit->setMinimumSize(QSize(60, 25));
        m_end_dateEdit->setFont(font1);
        m_end_dateEdit->setCalendarPopup(true);

        horizontalLayout->addWidget(m_end_dateEdit);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(50, 25));
        pushButton->setMaximumSize(QSize(65, 30));
        pushButton->setFont(font);

        horizontalLayout->addWidget(pushButton);

        pushButton_before_query = new QPushButton(groupBox);
        pushButton_before_query->setObjectName(QString::fromUtf8("pushButton_before_query"));
        pushButton_before_query->setMinimumSize(QSize(50, 25));
        pushButton_before_query->setMaximumSize(QSize(65, 30));
        pushButton_before_query->setFont(font);

        horizontalLayout->addWidget(pushButton_before_query);

        pushButton_next_query = new QPushButton(groupBox);
        pushButton_next_query->setObjectName(QString::fromUtf8("pushButton_next_query"));
        pushButton_next_query->setMinimumSize(QSize(50, 25));
        pushButton_next_query->setMaximumSize(QSize(65, 30));
        pushButton_next_query->setFont(font);

        horizontalLayout->addWidget(pushButton_next_query);

        label_page_query = new QLabel(groupBox);
        label_page_query->setObjectName(QString::fromUtf8("label_page_query"));
        label_page_query->setMinimumSize(QSize(0, 30));
        label_page_query->setMaximumSize(QSize(40, 30));
        label_page_query->setFont(font);

        horizontalLayout->addWidget(label_page_query);

        lineEdit_page_query = new QLineEdit(groupBox);
        lineEdit_page_query->setObjectName(QString::fromUtf8("lineEdit_page_query"));
        lineEdit_page_query->setMinimumSize(QSize(0, 30));
        lineEdit_page_query->setMaximumSize(QSize(45, 30));
        lineEdit_page_query->setFont(font);
        lineEdit_page_query->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit_page_query);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(groupBox);

        tableView = new QTableView(dlgSystemRecord);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(0, 212));
        tableView->setFont(font);
        tableView->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(tableView);

        horizontalLayout_page = new QHBoxLayout();
        horizontalLayout_page->setObjectName(QString::fromUtf8("horizontalLayout_page"));
        horizontalSpacer_page = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_page->addItem(horizontalSpacer_page);

        label_page = new QLabel(dlgSystemRecord);
        label_page->setObjectName(QString::fromUtf8("label_page"));
        label_page->setMinimumSize(QSize(0, 30));
        label_page->setMaximumSize(QSize(40, 30));
        label_page->setFont(font);

        horizontalLayout_page->addWidget(label_page);

        lineEdit_page = new QLineEdit(dlgSystemRecord);
        lineEdit_page->setObjectName(QString::fromUtf8("lineEdit_page"));
        lineEdit_page->setMinimumSize(QSize(0, 30));
        lineEdit_page->setMaximumSize(QSize(45, 30));
        lineEdit_page->setFont(font);
        lineEdit_page->setReadOnly(true);

        horizontalLayout_page->addWidget(lineEdit_page);

        pushButton_before = new QPushButton(dlgSystemRecord);
        pushButton_before->setObjectName(QString::fromUtf8("pushButton_before"));
        pushButton_before->setMinimumSize(QSize(50, 30));
        pushButton_before->setMaximumSize(QSize(65, 30));
        pushButton_before->setFont(font);

        horizontalLayout_page->addWidget(pushButton_before);

        pushButton_next = new QPushButton(dlgSystemRecord);
        pushButton_next->setObjectName(QString::fromUtf8("pushButton_next"));
        pushButton_next->setMinimumSize(QSize(50, 30));
        pushButton_next->setMaximumSize(QSize(65, 30));
        pushButton_next->setFont(font);

        horizontalLayout_page->addWidget(pushButton_next);


        verticalLayout_2->addLayout(horizontalLayout_page);


        retranslateUi(dlgSystemRecord);

        QMetaObject::connectSlotsByName(dlgSystemRecord);
    } // setupUi

    void retranslateUi(QDialog *dlgSystemRecord)
    {
        dlgSystemRecord->setWindowTitle(QApplication::translate("dlgSystemRecord", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("dlgSystemRecord", "\346\237\245\350\257\242\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgSystemRecord", "\347\261\273\345\236\213:", 0, QApplication::UnicodeUTF8));
        m_WorklogType->clear();
        m_WorklogType->insertItems(0, QStringList()
         << QApplication::translate("dlgSystemRecord", "\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("dlgSystemRecord", "\345\272\224\346\200\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("dlgSystemRecord", "\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("dlgSystemRecord", "\345\205\266\345\256\203", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("dlgSystemRecord", "\345\274\200\345\247\213\346\227\266\351\227\264:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("dlgSystemRecord", "\347\273\223\346\235\237\346\227\266\351\227\264:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("dlgSystemRecord", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        pushButton_before_query->setText(QApplication::translate("dlgSystemRecord", "\344\270\212\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        pushButton_next_query->setText(QApplication::translate("dlgSystemRecord", "\344\270\213\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        label_page_query->setText(QApplication::translate("dlgSystemRecord", "\351\241\265\347\240\201:", 0, QApplication::UnicodeUTF8));
        label_page->setText(QApplication::translate("dlgSystemRecord", "\351\241\265\347\240\201:", 0, QApplication::UnicodeUTF8));
        pushButton_before->setText(QApplication::translate("dlgSystemRecord", "\344\270\212\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        pushButton_next->setText(QApplication::translate("dlgSystemRecord", "\344\270\213\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgSystemRecord: public Ui_dlgSystemRecord {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSYSTEMRECORD_H
