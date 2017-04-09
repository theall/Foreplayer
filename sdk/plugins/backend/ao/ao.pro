#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT += core

TARGET = plugin
TEMPLATE = lib

DEFINES += 

LIBS += -LZ:/temp/build-aosdk-Desktop_Qt_5_5_1_MinGW_32bit-Debug/debug -laosdk
LIBS += -lunzip -lz

INCLUDEPATH += ../

SOURCES += \
    main.cpp \
    abstractparse.cpp \
    fileparse.cpp \
    zipparse.cpp

HEADERS += \
    abstractparse.h \
    fileparse.h \
    zipparse.h

win32 {
    DLLDESTDIR = Z:/build/foreplayer/debug/debug/plugins/backend/ao
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
