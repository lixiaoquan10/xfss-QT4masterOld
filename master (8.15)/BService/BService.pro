#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T12:53:43
#
#-------------------------------------------------

QT       += gui network sql svg xml

TARGET = BService
TEMPLATE = lib
TRANSLATIONS = BService_zh_CN.ts

INCLUDEPATH += /home/ubuntu1204/mqtt/result_dir/paho.mqtt.cpp_result/include
LIBS += -L/home/ubuntu1204/mqtt/result_dir/paho.mqtt.cpp_result/lib  -lpaho-mqttpp3
#INCLUDEPATH += /home/ubuntu1204/mqtt/result_dir/paho.mqtt.c_result/include
#LIBS += -L/home/ubuntu1204/mqtt/result_dir/paho.mqtt.c_result/lib -lpaho-mqtt3a

DEFINES += BSERVICE_LIBRARY
include("serial/qextserialport.pri")

SOURCES += bservice.cpp \
    cglobal.cpp \
    print/cprint.cpp \
    log/clog.cpp \
    dialog/dlginformationwindow.cpp \
    ctrace.cpp \
    process/cserialport.cpp \
    process/ccommandbytedata.cpp \
    process/ccommunicationmanager.cpp \
    process/cprocessbase.cpp \
    process/ccandatabytearray.cpp \
    process/packagetemplate.cpp \
    process/packageanalyzer.cpp \
    process/ccommunicationline.cpp \
    process/ccommunicationroute.cpp \
    process/cprocessdistribution.cpp \
    process/cprocesscontroller.cpp \
    struct/cmsgstructbase.cpp \
    struct/cmsgnull.cpp \
    struct/cmsgprocesstypename.cpp \
    cadapterobject.cpp \
    struct/cmsglog.cpp \
    struct/cmsgrouteconfig.cpp \
    struct/cmsgchecktime.cpp \
    process/cprocessclient.cpp \
    struct/cmsgloginfo.cpp \
    struct/cmsgprintset.cpp \
    struct/cmsgcheckset.cpp \
    struct/cmsgdirectiontest.cpp \
    struct/cmsgdeviceinfo.cpp \
    struct/cmsgevacuationgroup.cpp \
    struct/cmsgfirepoint.cpp \
    struct/cmsgpowerinfo.cpp \
    struct/cmsgobjectstatusinfo.cpp \
    struct/cmsgobjectstatus.cpp \
    struct/cmsgcontrollerinfo.cpp \
    struct/cmsglogin.cpp \
    struct/cmsgledinfo.cpp \
    cplatform_arm.cpp \
    process/cprocessmqtt.cpp

HEADERS += bservice.h \
    cglobal.h \
    print/cprint.h \
    log/clog.h \
    dialog/dlginformationwindow.h \
    struct.h \
    ctrace.h \
    process/cserialport.h \
    process/ccommandbytedata.h \
    process/ccommunicationmanager.h \
    process/cprocessbase.h \
    process/ccandatabytearray.h \
    process/packagetemplate.h \
    process/packageanalyzer.h \
    process/ccommunicationline.h \
    process/ccommunicationroute.h \
    process/cprocessdistribution.h \
    process/cprocesscontroller.h \
    struct/cmsgstructbase.h \
    struct/cmsgnull.h \
    struct/cmsgprocesstypename.h \
    cadapterobject.h \
    struct/cmsglog.h \
    struct/cmsgrouteconfig.h \
    struct/cmsgchecktime.h \
    process/cprocessclient.h \
    struct/cmsgloginfo.h \
    struct/cmsgprintset.h \
    struct/cmsgcheckset.h \
    struct/cmsgdirectiontest.h \
    struct/cmsgdeviceinfo.h \
    struct/cmsgevacuationgroup.h \
    struct/cmsgfirepoint.h \
    struct/cmsgpowerinfo.h \
    struct/cmsgobjectstatusinfo.h \
    struct/cmsgobjectstatus.h \
    struct/cmsgcontrollerinfo.h \
    struct/cmsglogin.h \
    struct/cmsgledinfo.h \
    cplatform_arm.h \
    process/cprocessmqtt.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE16B851A
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = BService.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /root
    } else {
        target.path = /root
    }
    INSTALLS += target
}

RESOURCES += \
    images.qrc
