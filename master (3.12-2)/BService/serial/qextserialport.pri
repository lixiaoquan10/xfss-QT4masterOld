INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS                += $$PWD/qextserialport.h \
                          $$PWD/qextserialport_p.h \
                          $$PWD/qextserialenumerator.h \
                          $$PWD/qextserialenumerator_p.h \
                          $$PWD/qextserialport_global.h
SOURCES                += $$PWD/qextserialport.cpp \
                          $$PWD/qextserialenumerator.cpp
unix:SOURCES           += $$PWD/qextserialport_unix.cpp
unix:!macx:SOURCES     += $$PWD/qextserialenumerator_unix.cpp
macx:SOURCES           += $$PWD/qextserialenumerator_osx.cpp
win32:SOURCES          += $$PWD/qextserialport_win.cpp \
                          $$PWD/qextserialenumerator_win.cpp

# For Windows user who doesn't have Qt4's Private files
win32:contains(QT_VERSION, ^4\\..*\\..*):!exists($$[QT_INSTALL_HEADERS]/QtCore/private/qwineventnotifier_p.h){
    DEFINES            += QESP_NO_QT4_PRIVATE
    HEADERS            += $$PWD/qextwineventnotifier_p.h
    SOURCES            += $$PWD/qextwineventnotifier_p.cpp
}


macx:LIBS              += -framework IOKit -framework CoreFoundation
win32:LIBS             += -lsetupapi -ladvapi32 -luser32
