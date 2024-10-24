#-------------------------------------------------
#
# Project created by QtCreator 2015-08-03T12:03:11
#
#-------------------------------------------------

QT += core gui sql svg xml
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Monitor
TEMPLATE = app
win32 {
    LIBS += -luser32
}

LIBS += -L../Monitor
TRANSLATIONS = Monitor_zh_CN.ts

#INCLUDEPATH += /home/ubuntu1204/mqtt/result_dir/paho.mqtt.cpp_result/include
#LIBS += -L/home/ubuntu1204/mqtt/result_dir/paho.mqtt.cpp_result/lib -lpaho-mqttpp3


SOURCES += main.cpp\
        widget.cpp \
    business/cdataprocessing.cpp \
    dm/ccontroller.cpp \
    dm/cobject.cpp \
    dm/cloop.cpp \
    dm/clayer.cpp \
    dm/cdm.cpp \
    dm/cdistribution.cpp \
    dm/cdevice.cpp \
    dm/cfirepoint.cpp \
    dm/cdbsqlite.cpp \
    dm/ccanport.cpp \
    graphics/cmath.cpp \
    model/ctreeitem.cpp \
    model/ctablemodel.cpp \
    model/ccontrollertreemodel.cpp \
    model/ccontrollermodel.cpp \
    model/cmodel.cpp \
    model/cloopmodel.cpp \
    model/cdistributionmodel.cpp \
    model/cdevicesfindtablemodel.cpp \
    model/cdevicemodel.cpp \
    model/cdesigntreemodel.cpp \
    model/ccanportmodel.cpp \
    view/ctableview.cpp \
    view/ccontrollertreeview.cpp \
    cglobal.cpp \
    cdevicetype.cpp \
    model/cinformationmodel.cpp \
    business/cclientbusiness.cpp \
    communication/ccommunication.cpp \
    widget/ctoptoolbar.cpp \
    widget/ccenterframe.cpp \
    widget/cbottomtoolbar.cpp \
    widget/cprogramview.cpp \
    widget/cprogramtreeview.cpp \
    widget/cprogramcontrollerview.cpp \
    widget/cprogramcandeviceview.cpp \
    widget/cprogramdistributionview.cpp \
    widget/cprogramdeviceview.cpp \
    widget/cprogramcanportview.cpp \
    widget/cprogramcontainer.cpp \
    widget/cinformationlabel.cpp \
    widget/cdesignview.cpp \
    widget/cdesigntreeview.cpp \
    widget/cdesignsplashview.cpp \
    widget/cdesigncontainer.cpp \
    dialog/dlgsystemrecordmodel.cpp \
    dialog/dlgsystemrecord.cpp \
    dialog/dlgadministrator.cpp \
    struct/cmsgstructbase.cpp \
    struct/cmsgrouteconfig.cpp \
    struct/cmsgprocesstypename.cpp \
    struct/cmsgprintset.cpp \
    struct/cmsglog.cpp \
    struct/cmsgchecktime.cpp \
    struct/cmsgcheckset.cpp \
    delegate/cpathdelegate.cpp \
    delegate/cdevicetypedelegate.cpp \
    style/cstylehelper.cpp \
    style/cstyledbar.cpp \
    style/cstyleanimator.cpp \
    style/cmanhattanstyle.cpp \
    struct/cmsgpowerinfo.cpp \
    struct/cmsgobjectstatusinfo.cpp \
    struct/cmsgobjectstatus.cpp \
    struct/cmsgnull.cpp \
    struct/cmsgfirepoint.cpp \
    struct/cmsgevacuationgroup.cpp \
    struct/cmsgdirectiontest.cpp \
    struct/cmsgdeviceinfo.cpp \
    struct/cmsgcontrollerinfo.cpp \
    dialog/dlginformationwindow.cpp \
    dialog/dlgsearchdevice.cpp \
    dialog/dlgchangeadd.cpp \
    dialog/dlgchangetype.cpp \
    dialog/dlghandfireview.cpp \
    groupbox.cpp \
    cfilterobject.cpp \
    cmyiconstyle.cpp \
    dialog/tablewidget.cpp \
    dialog/dlglogininformation.cpp \
    qcustomplot.cpp \
    dialog/dlgtimeinterval.cpp \
    struct/cmsglogin.cpp \
    struct/cmsgloginfo.cpp \
    struct/cmsgledinfo.cpp \
    keyboard/ckbcontext.cpp \
    keyboard/ckeyboard.cpp \
    dialog/dlgdevices.cpp \
    widget/cstatustabs.cpp \
    model/cdevicelistmodel.cpp \
    graphics/configfilemanager.cpp \
    dialog/dlgusetime.cpp \
    dialog/dlguploadlamp.cpp \
    dialog/dlgadminparam.cpp \
    dialog/dlgadminParamPage.cpp \
    widget/cminisplitter.cpp \
    graphics/deviceForbidFault.cpp \
    graphics/deviceControl.cpp \
    dialog/dlgdeviceForbidFault.cpp \
    model/cdeviceforbidfaultlistmodel.cpp \
    business/cprocesssocket.cpp \
    business/cprocessserver.cpp

HEADERS  += widget.h \
    business/cdataprocessing.h \
    dm/objectstatusenum.h \
    dm/ccontroller.h \
    dm/cobject.h \
    dm/cloop.h \
    dm/clayer.h \
    dm/cdm.h \
    dm/cdistribution.h \
    dm/cdevice.h \
    dm/cfirepoint.h \
    dm/cdbsqlite.h \
    dm/ccanport.h \
    graphics/cmath.h \
    model/ctreeitem.h \
    model/ctablemodel.h \
    model/ccontrollertreemodel.h \
    model/ccontrollermodel.h \
    model/cmodel.h \
    model/cloopmodel.h \
    model/cdistributionmodel.h \
    model/cdevicesfindtablemodel.h \
    model/cdevicemodel.h \
    model/cdesigntreemodel.h \
    model/ccanportmodel.h \
    view/ctableview.h \
    view/ccontrollertreeview.h \
    cglobal.h \
    cdevicetype.h \
    model/cinformationmodel.h \
    business/cclientbusiness.h \
    communication/ccommunication.h \
    widget/ctoptoolbar.h \
    widget/ccenterframe.h \
    widget/cbottomtoolbar.h \
    widget/cprogramview.h \
    widget/cprogramtreeview.h \
    widget/cprogramcontrollerview.h \
    widget/cprogramcandeviceview.h \
    widget/cprogramdistributionview.h \
    widget/cprogramdeviceview.h \
    widget/cprogramcanportview.h \
    widget/cprogramcontainer.h \
    widget/cminisplitter.h \
    widget/cinformationlabel.h \
    widget/cdesignview.h \
    widget/cdesigntreeview.h \
    widget/cdesignsplashview.h \
    widget/cdesigncontainer.h \
    dialog/dlgsystemrecordmodel.h \
    dialog/dlgsystemrecord.h \
    dialog/dlgadministrator.h \
    struct/shardEnum.h \
    struct/cmsgstructbase.h \
    struct/cmsgrouteconfig.h \
    struct/cmsgprocesstypename.h \
    struct/cmsgprintset.h \
    struct/cmsglog.h \
    struct/cmsgchecktime.h \
    struct/cmsgcheckset.h \
    struct/shardEnum.h \
    delegate/cpathdelegate.h \
    delegate/cdevicetypedelegate.h \
    style/cstylehelper.h \
    style/cstyledbar.h \
    style/cstyleanimator.h \
    style/cmanhattanstyle.h \
    struct/cmsgpowerinfo.h \
    struct/cmsgobjectstatusinfo.h \
    struct/cmsgobjectstatus.h \
    struct/cmsgnull.h \
    struct/cmsgfirepoint.h \
    struct/cmsgevacuationgroup.h \
    struct/cmsgdirectiontest.h \
    struct/cmsgdeviceinfo.h \
    struct/cmsgcontrollerinfo.h \
    dialog/dlginformationwindow.h \
    dialog/dlgsearchdevice.h \
    dialog/dlgchangeadd.h \
    dialog/dlgchangetype.h \
    dialog/dlghandfireview.h \
    groupbox.h \
    cfilterobject.h \
    cmyiconstyle.h \
    dialog/tablewidget.h \
    dialog/dlglogininformation.h \
    qcustomplot.h \
    dialog/dlgtimeinterval.h \
    struct/cmsglogin.h \
    struct/cmsgloginfo.h \
    struct/cmsgledinfo.h \
    keyboard/ckbcontext.h \
    keyboard/ckeyboard.h \
    dialog/dlgdevices.h \
    widget/cstatustabs.h \
    model/cdevicelistmodel.h \
    graphics/configfilemanager.h \
    dialog/dlgusetime.h \
    dialog/dlguploadlamp.h \
    dialog/dlgadminparam.h \
    dialog/dlgadminParamPage.h \
    graphics/deviceForbidFault.h \
    graphics/deviceControl.h \
    dialog/dlgdeviceForbidFault.h \
    model/cdeviceforbidfaultlistmodel.h \
    business/cprocesssocket.h \
    business/cprocessserver.h

FORMS += \
    widget/cprogramprojectview.ui \
    widget/cprogramcandeviceview.ui \
    widget/cprogramformulaview.ui \
    widget/cprogramdeviceview.ui \
    widget/cprogramcanportview.ui \
    dialog/dlgsystemrecord.ui \
    dialog/dlgadministrator.ui \
    dialog/dlgsearchdevice.ui \
    dialog/dlgtimeinterval.ui \
    dialog/dlghandfireview.ui \
    keyboard/ckeyboard.ui \
    dialog/dlgdevices.ui \
    dialog/dlgchangetype.ui \
    dialog/dlgchangeadd.ui \
    dialog/dlgusetime.ui \
    dialog/dlgadminparam.ui \
    dialog/dlgadminParamPage.ui \
    widget/cprogramdistributionview.ui \
    dialog/dlguploadlamp.ui \
    dialog/dlgdeviceForbidFault.ui

RESOURCES += \
    images.qrc

target.path=/home/xfss/root
INSTALLS += target

RC_FILE = Monitor.rc

