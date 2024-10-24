/********************************************************************************
** Form generated from reading UI file 'dlgtimeinterval.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGTIMEINTERVAL_H
#define UI_DLGTIMEINTERVAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dlgTimeInterval
{
public:
    QGroupBox *SoundBox;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *SoundFCheckBox;
    QCheckBox *SoundECheckBox;
    QGroupBox *MYcheckBox;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *MonthCheckBox;
    QCheckBox *YearCheckBox;
    QGroupBox *EnergyModeBox;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *EnergyCheckBox;
    QGroupBox *MYcheckTimeBox;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_7;
    QLabel *label_monthtime;
    QLabel *label_9;
    QSlider *MonthTimeSlider;
    QGroupBox *EmergencyBox;
    QVBoxLayout *verticalLayout_12;
    QCheckBox *DirectionTwinkleCheckBox;
    QCheckBox *UCDirectionTwinkleCheckBox;
    QCheckBox *TwinkleCheckBox;
    QGroupBox *VersionBox;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_10;
    QLineEdit *version_ledkey;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_11;
    QLineEdit *version_can1can2;
    QPushButton *pushButton_can1can2;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_15;
    QLineEdit *version_can3can4;
    QPushButton *pushButton_can3can4;
    QGroupBox *PowerBox;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *PowersendLayout;
    QLabel *label_12;
    QLabel *label_powersend;
    QHBoxLayout *PowerOLayout;
    QLabel *label_13;
    QLabel *label_powerotime;
    QHBoxLayout *PowerFaultLayout;
    QLabel *label_14;
    QLabel *label_powerfault;
    QGroupBox *SwitchLayerBox;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *SwitchLayerTime;
    QLabel *label_2;
    QGroupBox *hostIPSetBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_myip;
    QLineEdit *m_myip;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_gateway;
    QLineEdit *m_gateway;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_subnetMask;
    QLineEdit *m_subnetMask;
    QHBoxLayout *horizontalLayout_16;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_saveHostIP;
    QGroupBox *UseTimeBox;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_14;
    QLineEdit *UseTime;
    QPushButton *pushButton_UseTime;
    QGroupBox *timeBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QDateTimeEdit *dateEdit;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_5;
    QDateTimeEdit *timeEdit;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *m_ButtonTimeSet;
    QGroupBox *pingBox;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_6;
    QLineEdit *m_pingIP;
    QPushButton *pingButton;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_8;
    QLineEdit *m_pingResult;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *cloudIPSetBox;
    QVBoxLayout *verticalLayout_13;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_Hostip;
    QLineEdit *m_hostip;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_com;
    QLineEdit *m_com;
    QPushButton *pushButton_saveCloudIP;
    QGroupBox *comStatusBox;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_cloudStatus;
    QLineEdit *m_cloudStatus;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_masterStatus;
    QLineEdit *m_masterStatus;
    QGroupBox *StartRuleBox;
    QVBoxLayout *verticalLayout_15;
    QCheckBox *allStartCheckBox;
    QCheckBox *onlyStartPageCheckBox;
    QCheckBox *onlyStartGroupCheckBox;
    QGroupBox *CanNumberBox;
    QVBoxLayout *verticalLayout_16;
    QCheckBox *Can4CheckBox;
    QCheckBox *Can8CheckBox;

    void setupUi(QDialog *dlgTimeInterval)
    {
        if (dlgTimeInterval->objectName().isEmpty())
            dlgTimeInterval->setObjectName(QString::fromUtf8("dlgTimeInterval"));
        dlgTimeInterval->resize(1366, 768);
        dlgTimeInterval->setMinimumSize(QSize(1366, 768));
        dlgTimeInterval->setMaximumSize(QSize(1366, 768));
        QFont font;
        font.setPointSize(13);
        dlgTimeInterval->setFont(font);
        dlgTimeInterval->setInputMethodHints(Qt::ImhNone);
        SoundBox = new QGroupBox(dlgTimeInterval);
        SoundBox->setObjectName(QString::fromUtf8("SoundBox"));
        SoundBox->setGeometry(QRect(50, 20, 130, 80));
        QFont font1;
        font1.setPointSize(12);
        SoundBox->setFont(font1);
        SoundBox->setCheckable(false);
        SoundBox->setChecked(false);
        verticalLayout_2 = new QVBoxLayout(SoundBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        SoundFCheckBox = new QCheckBox(SoundBox);
        SoundFCheckBox->setObjectName(QString::fromUtf8("SoundFCheckBox"));
        SoundFCheckBox->setFont(font1);

        verticalLayout_2->addWidget(SoundFCheckBox);

        SoundECheckBox = new QCheckBox(SoundBox);
        SoundECheckBox->setObjectName(QString::fromUtf8("SoundECheckBox"));
        SoundECheckBox->setFont(font1);
        SoundECheckBox->setTristate(false);

        verticalLayout_2->addWidget(SoundECheckBox);

        MYcheckBox = new QGroupBox(dlgTimeInterval);
        MYcheckBox->setObjectName(QString::fromUtf8("MYcheckBox"));
        MYcheckBox->setGeometry(QRect(230, 20, 120, 80));
        MYcheckBox->setFont(font1);
        MYcheckBox->setCheckable(false);
        MYcheckBox->setChecked(false);
        verticalLayout_5 = new QVBoxLayout(MYcheckBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        MonthCheckBox = new QCheckBox(MYcheckBox);
        MonthCheckBox->setObjectName(QString::fromUtf8("MonthCheckBox"));
        MonthCheckBox->setFont(font1);

        verticalLayout_5->addWidget(MonthCheckBox);

        YearCheckBox = new QCheckBox(MYcheckBox);
        YearCheckBox->setObjectName(QString::fromUtf8("YearCheckBox"));
        YearCheckBox->setFont(font1);
        YearCheckBox->setTristate(false);

        verticalLayout_5->addWidget(YearCheckBox);

        EnergyModeBox = new QGroupBox(dlgTimeInterval);
        EnergyModeBox->setObjectName(QString::fromUtf8("EnergyModeBox"));
        EnergyModeBox->setGeometry(QRect(920, 20, 120, 60));
        EnergyModeBox->setFont(font1);
        EnergyModeBox->setCheckable(false);
        EnergyModeBox->setChecked(false);
        verticalLayout_6 = new QVBoxLayout(EnergyModeBox);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        EnergyCheckBox = new QCheckBox(EnergyModeBox);
        EnergyCheckBox->setObjectName(QString::fromUtf8("EnergyCheckBox"));
        EnergyCheckBox->setFont(font1);

        verticalLayout_6->addWidget(EnergyCheckBox);

        MYcheckTimeBox = new QGroupBox(dlgTimeInterval);
        MYcheckTimeBox->setObjectName(QString::fromUtf8("MYcheckTimeBox"));
        MYcheckTimeBox->setGeometry(QRect(350, 160, 140, 90));
        MYcheckTimeBox->setFont(font1);
        MYcheckTimeBox->setCheckable(false);
        MYcheckTimeBox->setChecked(false);
        verticalLayout_7 = new QVBoxLayout(MYcheckTimeBox);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_7 = new QLabel(MYcheckTimeBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(0, 28));
        label_7->setMaximumSize(QSize(84, 28));
        label_7->setFont(font1);

        horizontalLayout_12->addWidget(label_7);

        label_monthtime = new QLabel(MYcheckTimeBox);
        label_monthtime->setObjectName(QString::fromUtf8("label_monthtime"));
        label_monthtime->setMinimumSize(QSize(0, 28));
        label_monthtime->setMaximumSize(QSize(16777215, 28));
        label_monthtime->setFont(font1);
        label_monthtime->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(label_monthtime);

        label_9 = new QLabel(MYcheckTimeBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(0, 28));
        label_9->setMaximumSize(QSize(84, 28));
        label_9->setFont(font1);

        horizontalLayout_12->addWidget(label_9);


        verticalLayout_7->addLayout(horizontalLayout_12);

        MonthTimeSlider = new QSlider(MYcheckTimeBox);
        MonthTimeSlider->setObjectName(QString::fromUtf8("MonthTimeSlider"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MonthTimeSlider->sizePolicy().hasHeightForWidth());
        MonthTimeSlider->setSizePolicy(sizePolicy);
        MonthTimeSlider->setFont(font);
        MonthTimeSlider->setMinimum(30);
        MonthTimeSlider->setMaximum(180);
        MonthTimeSlider->setSingleStep(5);
        MonthTimeSlider->setOrientation(Qt::Horizontal);

        verticalLayout_7->addWidget(MonthTimeSlider);

        EmergencyBox = new QGroupBox(dlgTimeInterval);
        EmergencyBox->setObjectName(QString::fromUtf8("EmergencyBox"));
        EmergencyBox->setGeometry(QRect(400, 20, 260, 110));
        EmergencyBox->setFont(font1);
        EmergencyBox->setCheckable(false);
        EmergencyBox->setChecked(false);
        verticalLayout_12 = new QVBoxLayout(EmergencyBox);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        DirectionTwinkleCheckBox = new QCheckBox(EmergencyBox);
        DirectionTwinkleCheckBox->setObjectName(QString::fromUtf8("DirectionTwinkleCheckBox"));
        DirectionTwinkleCheckBox->setFont(font1);

        verticalLayout_12->addWidget(DirectionTwinkleCheckBox);

        UCDirectionTwinkleCheckBox = new QCheckBox(EmergencyBox);
        UCDirectionTwinkleCheckBox->setObjectName(QString::fromUtf8("UCDirectionTwinkleCheckBox"));
        UCDirectionTwinkleCheckBox->setFont(font1);

        verticalLayout_12->addWidget(UCDirectionTwinkleCheckBox);

        TwinkleCheckBox = new QCheckBox(EmergencyBox);
        TwinkleCheckBox->setObjectName(QString::fromUtf8("TwinkleCheckBox"));
        TwinkleCheckBox->setFont(font1);

        verticalLayout_12->addWidget(TwinkleCheckBox);

        VersionBox = new QGroupBox(dlgTimeInterval);
        VersionBox->setObjectName(QString::fromUtf8("VersionBox"));
        VersionBox->setGeometry(QRect(50, 130, 250, 140));
        VersionBox->setFont(font1);
        VersionBox->setCheckable(false);
        VersionBox->setChecked(false);
        verticalLayout_8 = new QVBoxLayout(VersionBox);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_10 = new QLabel(VersionBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(60, 28));
        label_10->setMaximumSize(QSize(16777215, 28));
        label_10->setFont(font1);

        horizontalLayout_13->addWidget(label_10);

        version_ledkey = new QLineEdit(VersionBox);
        version_ledkey->setObjectName(QString::fromUtf8("version_ledkey"));
        version_ledkey->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(version_ledkey->sizePolicy().hasHeightForWidth());
        version_ledkey->setSizePolicy(sizePolicy1);
        version_ledkey->setMinimumSize(QSize(0, 28));
        version_ledkey->setMaximumSize(QSize(90, 28));
        version_ledkey->setFont(font1);
        version_ledkey->setReadOnly(true);

        horizontalLayout_13->addWidget(version_ledkey);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_2);


        verticalLayout_8->addLayout(horizontalLayout_13);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        label_11 = new QLabel(VersionBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(60, 28));
        label_11->setMaximumSize(QSize(16777215, 28));
        label_11->setFont(font1);

        horizontalLayout_19->addWidget(label_11);

        version_can1can2 = new QLineEdit(VersionBox);
        version_can1can2->setObjectName(QString::fromUtf8("version_can1can2"));
        version_can1can2->setEnabled(true);
        version_can1can2->setMinimumSize(QSize(0, 28));
        version_can1can2->setMaximumSize(QSize(100, 28));
        version_can1can2->setFont(font1);
        version_can1can2->setReadOnly(true);

        horizontalLayout_19->addWidget(version_can1can2);

        pushButton_can1can2 = new QPushButton(VersionBox);
        pushButton_can1can2->setObjectName(QString::fromUtf8("pushButton_can1can2"));
        pushButton_can1can2->setMinimumSize(QSize(0, 28));
        pushButton_can1can2->setMaximumSize(QSize(60, 28));
        pushButton_can1can2->setFont(font1);

        horizontalLayout_19->addWidget(pushButton_can1can2);


        verticalLayout_8->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        label_15 = new QLabel(VersionBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(60, 28));
        label_15->setMaximumSize(QSize(16777215, 28));
        label_15->setFont(font1);

        horizontalLayout_20->addWidget(label_15);

        version_can3can4 = new QLineEdit(VersionBox);
        version_can3can4->setObjectName(QString::fromUtf8("version_can3can4"));
        version_can3can4->setEnabled(true);
        version_can3can4->setMinimumSize(QSize(0, 28));
        version_can3can4->setMaximumSize(QSize(100, 28));
        version_can3can4->setFont(font1);
        version_can3can4->setReadOnly(true);

        horizontalLayout_20->addWidget(version_can3can4);

        pushButton_can3can4 = new QPushButton(VersionBox);
        pushButton_can3can4->setObjectName(QString::fromUtf8("pushButton_can3can4"));
        pushButton_can3can4->setMinimumSize(QSize(0, 28));
        pushButton_can3can4->setMaximumSize(QSize(60, 28));
        pushButton_can3can4->setFont(font1);

        horizontalLayout_20->addWidget(pushButton_can3can4);


        verticalLayout_8->addLayout(horizontalLayout_20);

        PowerBox = new QGroupBox(dlgTimeInterval);
        PowerBox->setObjectName(QString::fromUtf8("PowerBox"));
        PowerBox->setGeometry(QRect(890, 630, 111, 31));
        PowerBox->setCheckable(false);
        PowerBox->setChecked(false);
        verticalLayout_11 = new QVBoxLayout(PowerBox);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        PowersendLayout = new QHBoxLayout();
        PowersendLayout->setObjectName(QString::fromUtf8("PowersendLayout"));
        label_12 = new QLabel(PowerBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMaximumSize(QSize(84, 16777215));

        PowersendLayout->addWidget(label_12);

        label_powersend = new QLabel(PowerBox);
        label_powersend->setObjectName(QString::fromUtf8("label_powersend"));
        label_powersend->setAlignment(Qt::AlignCenter);

        PowersendLayout->addWidget(label_powersend);


        verticalLayout_11->addLayout(PowersendLayout);

        PowerOLayout = new QHBoxLayout();
        PowerOLayout->setObjectName(QString::fromUtf8("PowerOLayout"));
        label_13 = new QLabel(PowerBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMaximumSize(QSize(84, 16777215));

        PowerOLayout->addWidget(label_13);

        label_powerotime = new QLabel(PowerBox);
        label_powerotime->setObjectName(QString::fromUtf8("label_powerotime"));
        label_powerotime->setAlignment(Qt::AlignCenter);

        PowerOLayout->addWidget(label_powerotime);


        verticalLayout_11->addLayout(PowerOLayout);

        PowerFaultLayout = new QHBoxLayout();
        PowerFaultLayout->setObjectName(QString::fromUtf8("PowerFaultLayout"));
        label_14 = new QLabel(PowerBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMaximumSize(QSize(84, 16777215));

        PowerFaultLayout->addWidget(label_14);

        label_powerfault = new QLabel(PowerBox);
        label_powerfault->setObjectName(QString::fromUtf8("label_powerfault"));
        label_powerfault->setAlignment(Qt::AlignCenter);

        PowerFaultLayout->addWidget(label_powerfault);


        verticalLayout_11->addLayout(PowerFaultLayout);

        SwitchLayerBox = new QGroupBox(dlgTimeInterval);
        SwitchLayerBox->setObjectName(QString::fromUtf8("SwitchLayerBox"));
        SwitchLayerBox->setGeometry(QRect(670, 630, 121, 31));
        SwitchLayerBox->setCheckable(true);
        SwitchLayerBox->setChecked(false);
        horizontalLayout_3 = new QHBoxLayout(SwitchLayerBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(SwitchLayerBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        SwitchLayerTime = new QLineEdit(SwitchLayerBox);
        SwitchLayerTime->setObjectName(QString::fromUtf8("SwitchLayerTime"));
        SwitchLayerTime->setMaximumSize(QSize(95, 16777215));

        horizontalLayout_2->addWidget(SwitchLayerTime);

        label_2 = new QLabel(SwitchLayerBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);


        horizontalLayout_3->addLayout(horizontalLayout_2);

        hostIPSetBox = new QGroupBox(dlgTimeInterval);
        hostIPSetBox->setObjectName(QString::fromUtf8("hostIPSetBox"));
        hostIPSetBox->setGeometry(QRect(1030, 140, 270, 170));
        hostIPSetBox->setFont(font1);
        verticalLayout_3 = new QVBoxLayout(hostIPSetBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_myip = new QLabel(hostIPSetBox);
        label_myip->setObjectName(QString::fromUtf8("label_myip"));
        label_myip->setMinimumSize(QSize(83, 28));
        label_myip->setMaximumSize(QSize(83, 28));
        label_myip->setFont(font1);

        horizontalLayout->addWidget(label_myip);

        m_myip = new QLineEdit(hostIPSetBox);
        m_myip->setObjectName(QString::fromUtf8("m_myip"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_myip->sizePolicy().hasHeightForWidth());
        m_myip->setSizePolicy(sizePolicy2);
        m_myip->setMinimumSize(QSize(150, 28));
        m_myip->setMaximumSize(QSize(150, 28));
        m_myip->setFont(font1);
        m_myip->setEchoMode(QLineEdit::Normal);

        horizontalLayout->addWidget(m_myip);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_gateway = new QLabel(hostIPSetBox);
        label_gateway->setObjectName(QString::fromUtf8("label_gateway"));
        label_gateway->setMinimumSize(QSize(83, 28));
        label_gateway->setMaximumSize(QSize(83, 28));
        label_gateway->setFont(font1);

        horizontalLayout_15->addWidget(label_gateway);

        m_gateway = new QLineEdit(hostIPSetBox);
        m_gateway->setObjectName(QString::fromUtf8("m_gateway"));
        m_gateway->setEnabled(true);
        sizePolicy2.setHeightForWidth(m_gateway->sizePolicy().hasHeightForWidth());
        m_gateway->setSizePolicy(sizePolicy2);
        m_gateway->setMinimumSize(QSize(150, 28));
        m_gateway->setMaximumSize(QSize(150, 28));
        m_gateway->setFont(font1);
        m_gateway->setEchoMode(QLineEdit::Normal);

        horizontalLayout_15->addWidget(m_gateway);


        verticalLayout_3->addLayout(horizontalLayout_15);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_subnetMask = new QLabel(hostIPSetBox);
        label_subnetMask->setObjectName(QString::fromUtf8("label_subnetMask"));
        label_subnetMask->setMinimumSize(QSize(83, 28));
        label_subnetMask->setMaximumSize(QSize(83, 28));
        label_subnetMask->setFont(font1);

        horizontalLayout_4->addWidget(label_subnetMask);

        m_subnetMask = new QLineEdit(hostIPSetBox);
        m_subnetMask->setObjectName(QString::fromUtf8("m_subnetMask"));
        sizePolicy2.setHeightForWidth(m_subnetMask->sizePolicy().hasHeightForWidth());
        m_subnetMask->setSizePolicy(sizePolicy2);
        m_subnetMask->setMinimumSize(QSize(150, 28));
        m_subnetMask->setMaximumSize(QSize(150, 28));
        m_subnetMask->setFont(font1);
        m_subnetMask->setEchoMode(QLineEdit::Normal);

        horizontalLayout_4->addWidget(m_subnetMask);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer);

        pushButton_saveHostIP = new QPushButton(hostIPSetBox);
        pushButton_saveHostIP->setObjectName(QString::fromUtf8("pushButton_saveHostIP"));
        pushButton_saveHostIP->setMinimumSize(QSize(0, 28));
        pushButton_saveHostIP->setMaximumSize(QSize(16777215, 28));
        pushButton_saveHostIP->setFont(font1);

        horizontalLayout_16->addWidget(pushButton_saveHostIP);


        verticalLayout_3->addLayout(horizontalLayout_16);

        UseTimeBox = new QGroupBox(dlgTimeInterval);
        UseTimeBox->setObjectName(QString::fromUtf8("UseTimeBox"));
        UseTimeBox->setGeometry(QRect(540, 160, 200, 70));
        UseTimeBox->setFont(font1);
        UseTimeBox->setCheckable(false);
        UseTimeBox->setChecked(false);
        verticalLayout_9 = new QVBoxLayout(UseTimeBox);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        UseTime = new QLineEdit(UseTimeBox);
        UseTime->setObjectName(QString::fromUtf8("UseTime"));
        UseTime->setEnabled(true);
        UseTime->setMinimumSize(QSize(0, 28));
        UseTime->setMaximumSize(QSize(40, 28));
        UseTime->setFont(font1);
        UseTime->setReadOnly(true);

        horizontalLayout_14->addWidget(UseTime);

        pushButton_UseTime = new QPushButton(UseTimeBox);
        pushButton_UseTime->setObjectName(QString::fromUtf8("pushButton_UseTime"));
        pushButton_UseTime->setMinimumSize(QSize(0, 28));
        pushButton_UseTime->setMaximumSize(QSize(16777215, 28));
        pushButton_UseTime->setFont(font1);

        horizontalLayout_14->addWidget(pushButton_UseTime);


        verticalLayout_9->addLayout(horizontalLayout_14);

        timeBox = new QGroupBox(dlgTimeInterval);
        timeBox->setObjectName(QString::fromUtf8("timeBox"));
        timeBox->setGeometry(QRect(790, 160, 190, 140));
        timeBox->setFont(font1);
        verticalLayout_4 = new QVBoxLayout(timeBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_4 = new QLabel(timeBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(0, 28));
        label_4->setMaximumSize(QSize(16777215, 28));
        label_4->setFont(font1);

        horizontalLayout_7->addWidget(label_4);

        dateEdit = new QDateTimeEdit(timeBox);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setMinimumSize(QSize(115, 28));
        dateEdit->setMaximumSize(QSize(115, 28));
        dateEdit->setFont(font1);

        horizontalLayout_7->addWidget(dateEdit);


        verticalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_5 = new QLabel(timeBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(0, 28));
        label_5->setMaximumSize(QSize(16777215, 28));
        label_5->setFont(font1);

        horizontalLayout_8->addWidget(label_5);

        timeEdit = new QDateTimeEdit(timeBox);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setMinimumSize(QSize(115, 28));
        timeEdit->setMaximumSize(QSize(115, 28));
        timeEdit->setFont(font1);

        horizontalLayout_8->addWidget(timeEdit);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_3 = new QSpacerItem(40, 30, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_3);

        m_ButtonTimeSet = new QPushButton(timeBox);
        m_ButtonTimeSet->setObjectName(QString::fromUtf8("m_ButtonTimeSet"));
        m_ButtonTimeSet->setMinimumSize(QSize(0, 28));
        m_ButtonTimeSet->setMaximumSize(QSize(16777215, 28));
        m_ButtonTimeSet->setFont(font1);

        horizontalLayout_9->addWidget(m_ButtonTimeSet);


        verticalLayout_4->addLayout(horizontalLayout_9);

        pingBox = new QGroupBox(dlgTimeInterval);
        pingBox->setObjectName(QString::fromUtf8("pingBox"));
        pingBox->setGeometry(QRect(50, 300, 270, 100));
        pingBox->setFont(font1);
        verticalLayout_10 = new QVBoxLayout(pingBox);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_6 = new QLabel(pingBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(0, 28));
        label_6->setMaximumSize(QSize(16777215, 28));
        label_6->setFont(font1);

        horizontalLayout_10->addWidget(label_6);

        m_pingIP = new QLineEdit(pingBox);
        m_pingIP->setObjectName(QString::fromUtf8("m_pingIP"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(m_pingIP->sizePolicy().hasHeightForWidth());
        m_pingIP->setSizePolicy(sizePolicy3);
        m_pingIP->setMinimumSize(QSize(0, 28));
        m_pingIP->setMaximumSize(QSize(150, 28));
        m_pingIP->setFont(font1);
        m_pingIP->setEchoMode(QLineEdit::Normal);

        horizontalLayout_10->addWidget(m_pingIP);

        pingButton = new QPushButton(pingBox);
        pingButton->setObjectName(QString::fromUtf8("pingButton"));
        pingButton->setMinimumSize(QSize(0, 28));
        pingButton->setMaximumSize(QSize(45, 28));
        pingButton->setFont(font1);

        horizontalLayout_10->addWidget(pingButton);


        verticalLayout_10->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_8 = new QLabel(pingBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(0, 28));
        label_8->setMaximumSize(QSize(16777215, 28));
        label_8->setFont(font1);

        horizontalLayout_11->addWidget(label_8);

        m_pingResult = new QLineEdit(pingBox);
        m_pingResult->setObjectName(QString::fromUtf8("m_pingResult"));
        sizePolicy3.setHeightForWidth(m_pingResult->sizePolicy().hasHeightForWidth());
        m_pingResult->setSizePolicy(sizePolicy3);
        m_pingResult->setMinimumSize(QSize(60, 28));
        m_pingResult->setMaximumSize(QSize(60, 28));
        m_pingResult->setFont(font1);
        m_pingResult->setEchoMode(QLineEdit::Normal);
        m_pingResult->setReadOnly(true);

        horizontalLayout_11->addWidget(m_pingResult);

        horizontalSpacer_4 = new QSpacerItem(38, 30, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_4);


        verticalLayout_10->addLayout(horizontalLayout_11);

        cloudIPSetBox = new QGroupBox(dlgTimeInterval);
        cloudIPSetBox->setObjectName(QString::fromUtf8("cloudIPSetBox"));
        cloudIPSetBox->setGeometry(QRect(370, 310, 230, 100));
        cloudIPSetBox->setFont(font1);
        verticalLayout_13 = new QVBoxLayout(cloudIPSetBox);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        label_Hostip = new QLabel(cloudIPSetBox);
        label_Hostip->setObjectName(QString::fromUtf8("label_Hostip"));
        label_Hostip->setMinimumSize(QSize(50, 28));
        label_Hostip->setMaximumSize(QSize(50, 28));
        label_Hostip->setFont(font1);

        horizontalLayout_17->addWidget(label_Hostip);

        m_hostip = new QLineEdit(cloudIPSetBox);
        m_hostip->setObjectName(QString::fromUtf8("m_hostip"));
        sizePolicy3.setHeightForWidth(m_hostip->sizePolicy().hasHeightForWidth());
        m_hostip->setSizePolicy(sizePolicy3);
        m_hostip->setMinimumSize(QSize(150, 28));
        m_hostip->setMaximumSize(QSize(150, 28));
        m_hostip->setFont(font1);
        m_hostip->setEchoMode(QLineEdit::Normal);

        horizontalLayout_17->addWidget(m_hostip);


        verticalLayout_13->addLayout(horizontalLayout_17);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_com = new QLabel(cloudIPSetBox);
        label_com->setObjectName(QString::fromUtf8("label_com"));
        label_com->setMinimumSize(QSize(50, 28));
        label_com->setMaximumSize(QSize(50, 28));
        label_com->setFont(font1);

        horizontalLayout_5->addWidget(label_com);

        m_com = new QLineEdit(cloudIPSetBox);
        m_com->setObjectName(QString::fromUtf8("m_com"));
        sizePolicy3.setHeightForWidth(m_com->sizePolicy().hasHeightForWidth());
        m_com->setSizePolicy(sizePolicy3);
        m_com->setMinimumSize(QSize(0, 28));
        m_com->setMaximumSize(QSize(60, 28));
        m_com->setFont(font1);
        m_com->setEchoMode(QLineEdit::Normal);

        horizontalLayout_5->addWidget(m_com);

        pushButton_saveCloudIP = new QPushButton(cloudIPSetBox);
        pushButton_saveCloudIP->setObjectName(QString::fromUtf8("pushButton_saveCloudIP"));
        pushButton_saveCloudIP->setMinimumSize(QSize(0, 28));
        pushButton_saveCloudIP->setMaximumSize(QSize(16777215, 28));
        pushButton_saveCloudIP->setFont(font1);

        horizontalLayout_5->addWidget(pushButton_saveCloudIP);


        verticalLayout_13->addLayout(horizontalLayout_5);

        comStatusBox = new QGroupBox(dlgTimeInterval);
        comStatusBox->setObjectName(QString::fromUtf8("comStatusBox"));
        comStatusBox->setGeometry(QRect(650, 310, 190, 100));
        comStatusBox->setFont(font1);
        verticalLayout_14 = new QVBoxLayout(comStatusBox);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_cloudStatus = new QLabel(comStatusBox);
        label_cloudStatus->setObjectName(QString::fromUtf8("label_cloudStatus"));
        label_cloudStatus->setMinimumSize(QSize(0, 28));
        label_cloudStatus->setMaximumSize(QSize(16777215, 28));
        label_cloudStatus->setFont(font1);

        horizontalLayout_6->addWidget(label_cloudStatus);

        m_cloudStatus = new QLineEdit(comStatusBox);
        m_cloudStatus->setObjectName(QString::fromUtf8("m_cloudStatus"));
        sizePolicy3.setHeightForWidth(m_cloudStatus->sizePolicy().hasHeightForWidth());
        m_cloudStatus->setSizePolicy(sizePolicy3);
        m_cloudStatus->setMinimumSize(QSize(0, 28));
        m_cloudStatus->setMaximumSize(QSize(80, 28));
        m_cloudStatus->setFont(font1);
        m_cloudStatus->setEchoMode(QLineEdit::Normal);
        m_cloudStatus->setReadOnly(true);

        horizontalLayout_6->addWidget(m_cloudStatus);


        verticalLayout_14->addLayout(horizontalLayout_6);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_masterStatus = new QLabel(comStatusBox);
        label_masterStatus->setObjectName(QString::fromUtf8("label_masterStatus"));
        label_masterStatus->setMinimumSize(QSize(0, 28));
        label_masterStatus->setMaximumSize(QSize(16777215, 28));
        label_masterStatus->setFont(font1);

        horizontalLayout_18->addWidget(label_masterStatus);

        m_masterStatus = new QLineEdit(comStatusBox);
        m_masterStatus->setObjectName(QString::fromUtf8("m_masterStatus"));
        sizePolicy3.setHeightForWidth(m_masterStatus->sizePolicy().hasHeightForWidth());
        m_masterStatus->setSizePolicy(sizePolicy3);
        m_masterStatus->setMinimumSize(QSize(0, 28));
        m_masterStatus->setMaximumSize(QSize(80, 28));
        m_masterStatus->setFont(font1);
        m_masterStatus->setEchoMode(QLineEdit::Normal);
        m_masterStatus->setReadOnly(true);

        horizontalLayout_18->addWidget(m_masterStatus);


        verticalLayout_14->addLayout(horizontalLayout_18);

        StartRuleBox = new QGroupBox(dlgTimeInterval);
        StartRuleBox->setObjectName(QString::fromUtf8("StartRuleBox"));
        StartRuleBox->setGeometry(QRect(710, 20, 160, 110));
        StartRuleBox->setFont(font1);
        StartRuleBox->setCheckable(false);
        StartRuleBox->setChecked(false);
        verticalLayout_15 = new QVBoxLayout(StartRuleBox);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        allStartCheckBox = new QCheckBox(StartRuleBox);
        allStartCheckBox->setObjectName(QString::fromUtf8("allStartCheckBox"));
        allStartCheckBox->setFont(font1);

        verticalLayout_15->addWidget(allStartCheckBox);

        onlyStartPageCheckBox = new QCheckBox(StartRuleBox);
        onlyStartPageCheckBox->setObjectName(QString::fromUtf8("onlyStartPageCheckBox"));
        onlyStartPageCheckBox->setFont(font1);

        verticalLayout_15->addWidget(onlyStartPageCheckBox);

        onlyStartGroupCheckBox = new QCheckBox(StartRuleBox);
        onlyStartGroupCheckBox->setObjectName(QString::fromUtf8("onlyStartGroupCheckBox"));
        onlyStartGroupCheckBox->setFont(font1);

        verticalLayout_15->addWidget(onlyStartGroupCheckBox);

        CanNumberBox = new QGroupBox(dlgTimeInterval);
        CanNumberBox->setObjectName(QString::fromUtf8("CanNumberBox"));
        CanNumberBox->setGeometry(QRect(1090, 20, 120, 80));
        CanNumberBox->setFont(font1);
        CanNumberBox->setCheckable(false);
        CanNumberBox->setChecked(false);
        verticalLayout_16 = new QVBoxLayout(CanNumberBox);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        Can4CheckBox = new QCheckBox(CanNumberBox);
        Can4CheckBox->setObjectName(QString::fromUtf8("Can4CheckBox"));
        Can4CheckBox->setFont(font1);

        verticalLayout_16->addWidget(Can4CheckBox);

        Can8CheckBox = new QCheckBox(CanNumberBox);
        Can8CheckBox->setObjectName(QString::fromUtf8("Can8CheckBox"));
        Can8CheckBox->setFont(font1);

        verticalLayout_16->addWidget(Can8CheckBox);

        SoundBox->raise();
        SwitchLayerBox->raise();
        MYcheckBox->raise();
        EnergyModeBox->raise();
        MYcheckTimeBox->raise();
        EmergencyBox->raise();
        VersionBox->raise();
        PowerBox->raise();
        hostIPSetBox->raise();
        UseTimeBox->raise();
        timeBox->raise();
        pingBox->raise();
        cloudIPSetBox->raise();
        comStatusBox->raise();
        StartRuleBox->raise();
        CanNumberBox->raise();

        retranslateUi(dlgTimeInterval);

        QMetaObject::connectSlotsByName(dlgTimeInterval);
    } // setupUi

    void retranslateUi(QDialog *dlgTimeInterval)
    {
        dlgTimeInterval->setWindowTitle(QApplication::translate("dlgTimeInterval", "Dialog", 0, QApplication::UnicodeUTF8));
        SoundBox->setTitle(QApplication::translate("dlgTimeInterval", "\345\243\260\351\237\263\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        SoundFCheckBox->setText(QApplication::translate("dlgTimeInterval", "\347\246\201\346\255\242\346\225\205\351\232\234\345\243\260", 0, QApplication::UnicodeUTF8));
        SoundECheckBox->setText(QApplication::translate("dlgTimeInterval", "\347\246\201\346\255\242\345\272\224\346\200\245\345\243\260", 0, QApplication::UnicodeUTF8));
        MYcheckBox->setTitle(QApplication::translate("dlgTimeInterval", "\346\234\210\346\243\200\345\271\264\346\243\200\345\274\200\345\205\263", 0, QApplication::UnicodeUTF8));
        MonthCheckBox->setText(QApplication::translate("dlgTimeInterval", "\347\246\201\346\255\242\346\234\210\346\243\200", 0, QApplication::UnicodeUTF8));
        YearCheckBox->setText(QApplication::translate("dlgTimeInterval", "\347\246\201\346\255\242\345\271\264\346\243\200", 0, QApplication::UnicodeUTF8));
        EnergyModeBox->setTitle(QApplication::translate("dlgTimeInterval", "\350\212\202\350\203\275\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        EnergyCheckBox->setText(QApplication::translate("dlgTimeInterval", "\350\212\202\350\203\275\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        MYcheckTimeBox->setTitle(QApplication::translate("dlgTimeInterval", "\346\234\210\346\243\200\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("dlgTimeInterval", "\346\234\210\346\243\200\346\214\201\347\273\255:", 0, QApplication::UnicodeUTF8));
        label_monthtime->setText(QApplication::translate("dlgTimeInterval", "0", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("dlgTimeInterval", "\347\247\222", 0, QApplication::UnicodeUTF8));
        EmergencyBox->setTitle(QApplication::translate("dlgTimeInterval", "\345\272\224\346\200\245\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        DirectionTwinkleCheckBox->setText(QApplication::translate("dlgTimeInterval", "\346\231\272\350\203\275\345\236\213\347\201\257\345\205\267\346\226\271\345\220\221\345\222\214\351\227\252\347\203\201\350\277\236\345\217\221", 0, QApplication::UnicodeUTF8));
        UCDirectionTwinkleCheckBox->setText(QApplication::translate("dlgTimeInterval", "\351\235\236\346\231\272\350\203\275\345\236\213\347\201\257\345\205\267\346\226\271\345\220\221\345\222\214\351\227\252\347\203\201\350\277\236\345\217\221", 0, QApplication::UnicodeUTF8));
        TwinkleCheckBox->setText(QApplication::translate("dlgTimeInterval", "\346\226\271\345\220\221\347\201\257\345\272\224\346\200\245\351\227\252\347\203\201\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        VersionBox->setTitle(QApplication::translate("dlgTimeInterval", "\344\270\255\351\227\264\345\261\202\347\211\210\346\234\254\345\217\267", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("dlgTimeInterval", "\347\201\257\351\224\256\346\235\277:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("dlgTimeInterval", "CAN1/2:", 0, QApplication::UnicodeUTF8));
        pushButton_can1can2->setText(QApplication::translate("dlgTimeInterval", "\350\257\273\345\217\226", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("dlgTimeInterval", "CAN3/4:", 0, QApplication::UnicodeUTF8));
        pushButton_can3can4->setText(QApplication::translate("dlgTimeInterval", "\350\257\273\345\217\226", 0, QApplication::UnicodeUTF8));
        PowerBox->setTitle(QApplication::translate("dlgTimeInterval", "PowerBox", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("dlgTimeInterval", "PowerSend:", 0, QApplication::UnicodeUTF8));
        label_powersend->setText(QApplication::translate("dlgTimeInterval", "0", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("dlgTimeInterval", "PowerOvertime:", 0, QApplication::UnicodeUTF8));
        label_powerotime->setText(QApplication::translate("dlgTimeInterval", "0", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("dlgTimeInterval", "PowerFault:", 0, QApplication::UnicodeUTF8));
        label_powerfault->setText(QApplication::translate("dlgTimeInterval", "0", 0, QApplication::UnicodeUTF8));
        SwitchLayerBox->setTitle(QApplication::translate("dlgTimeInterval", "Available", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlgTimeInterval", "Automatic Switch Layer:", 0, QApplication::UnicodeUTF8));
        SwitchLayerTime->setText(QString());
        label_2->setText(QApplication::translate("dlgTimeInterval", "Second", 0, QApplication::UnicodeUTF8));
        hostIPSetBox->setTitle(QApplication::translate("dlgTimeInterval", "\346\234\254\346\234\272IP", 0, QApplication::UnicodeUTF8));
        label_myip->setText(QApplication::translate("dlgTimeInterval", "\346\234\254\346\234\272IP\345\234\260\345\235\200:", 0, QApplication::UnicodeUTF8));
        label_gateway->setText(QApplication::translate("dlgTimeInterval", "\347\275\221        \345\205\263: ", 0, QApplication::UnicodeUTF8));
        label_subnetMask->setText(QApplication::translate("dlgTimeInterval", "\345\255\220 \347\275\221 \346\216\251 \347\240\201: ", 0, QApplication::UnicodeUTF8));
        pushButton_saveHostIP->setText(QApplication::translate("dlgTimeInterval", "\344\277\235\345\255\230\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        UseTimeBox->setTitle(QApplication::translate("dlgTimeInterval", "\344\275\277\347\224\250\346\234\237\351\231\220", 0, QApplication::UnicodeUTF8));
        pushButton_UseTime->setText(QApplication::translate("dlgTimeInterval", "\344\277\256\346\224\271\344\275\277\347\224\250\346\234\237\351\231\220", 0, QApplication::UnicodeUTF8));
        timeBox->setTitle(QApplication::translate("dlgTimeInterval", "\346\227\266\351\227\264\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("dlgTimeInterval", "\346\227\245\346\234\237:", 0, QApplication::UnicodeUTF8));
        dateEdit->setDisplayFormat(QApplication::translate("dlgTimeInterval", "yyyy-MM-dd", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("dlgTimeInterval", "\346\227\266\351\227\264:", 0, QApplication::UnicodeUTF8));
        timeEdit->setDisplayFormat(QApplication::translate("dlgTimeInterval", "HH:mm:ss", 0, QApplication::UnicodeUTF8));
        m_ButtonTimeSet->setText(QApplication::translate("dlgTimeInterval", "\344\277\256\346\224\271\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        pingBox->setTitle(QApplication::translate("dlgTimeInterval", "\351\200\232\350\256\257\346\265\213\350\257\225(Ping)", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("dlgTimeInterval", "IP\345\234\260\345\235\200:", 0, QApplication::UnicodeUTF8));
        pingButton->setText(QApplication::translate("dlgTimeInterval", "ping", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("dlgTimeInterval", "\346\265\213\350\257\225\346\210\220\345\212\237\347\216\207:", 0, QApplication::UnicodeUTF8));
        cloudIPSetBox->setTitle(QApplication::translate("dlgTimeInterval", "\344\272\221\345\271\263\345\217\260IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        label_Hostip->setText(QApplication::translate("dlgTimeInterval", "IP\345\234\260\345\235\200:", 0, QApplication::UnicodeUTF8));
        label_com->setText(QApplication::translate("dlgTimeInterval", "\347\253\257\345\217\243:", 0, QApplication::UnicodeUTF8));
        pushButton_saveCloudIP->setText(QApplication::translate("dlgTimeInterval", "\344\277\235\345\255\230\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        comStatusBox->setTitle(QApplication::translate("dlgTimeInterval", "\351\200\232\350\256\257\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        label_cloudStatus->setText(QApplication::translate("dlgTimeInterval", "\344\272\221\345\271\263\345\217\260:", 0, QApplication::UnicodeUTF8));
        label_masterStatus->setText(QApplication::translate("dlgTimeInterval", "\345\256\242\346\210\267\347\253\257:", 0, QApplication::UnicodeUTF8));
        StartRuleBox->setTitle(QApplication::translate("dlgTimeInterval", "\345\220\257\345\212\250\350\247\204\345\210\231", 0, QApplication::UnicodeUTF8));
        allStartCheckBox->setText(QApplication::translate("dlgTimeInterval", "\345\205\250\351\203\250\345\220\257\345\212\250", 0, QApplication::UnicodeUTF8));
        onlyStartPageCheckBox->setText(QApplication::translate("dlgTimeInterval", "\345\220\257\345\212\250\346\211\200\345\234\250\351\241\265\351\235\242", 0, QApplication::UnicodeUTF8));
        onlyStartGroupCheckBox->setText(QApplication::translate("dlgTimeInterval", "\345\210\206\347\273\204\345\220\257\345\212\250", 0, QApplication::UnicodeUTF8));
        CanNumberBox->setTitle(QApplication::translate("dlgTimeInterval", "CAN\345\233\236\350\267\257\346\225\260", 0, QApplication::UnicodeUTF8));
        Can4CheckBox->setText(QApplication::translate("dlgTimeInterval", "4\350\267\257CAN", 0, QApplication::UnicodeUTF8));
        Can8CheckBox->setText(QApplication::translate("dlgTimeInterval", "8\350\267\257CAN", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgTimeInterval: public Ui_dlgTimeInterval {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGTIMEINTERVAL_H
