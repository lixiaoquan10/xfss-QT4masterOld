INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS +=  $$PWD/libwmf/WmfStructs.h \
            $$PWD/libwmf/wmfstruct.h \
            $$PWD/libwmf/WmfStack.h \
            $$PWD/libwmf/WmfParser.h \
            $$PWD/libwmf/WmfPainterBackend.h \
            $$PWD/libwmf/WmfEnums.h \
            $$PWD/libwmf/WmfDeviceContext.h \
            $$PWD/libwmf/WmfAbstractBackend.h \
            $$PWD/libwmf/WmfWriter.h
SOURCES +=  $$PWD/libwmf/WmfWriter.cpp \
            $$PWD/libwmf/WmfStack.cpp \
            $$PWD/libwmf/WmfParser.cpp \
            $$PWD/libwmf/WmfPainterBackend.cpp \
            $$PWD/libwmf/WmfDeviceContext.cpp \
            $$PWD/libwmf/WmfAbstractBackend.cpp
