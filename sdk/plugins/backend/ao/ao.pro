#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT += core

TARGET = plugin
TEMPLATE = lib
CONFIG += c++11

DEFINES += 

LIBS += -LZ:/temp/build-aosdk-Desktop_Qt_5_5_1_MinGW_32bit-Debug/debug -laosdk
LIBS += -lunzip -lz

INCLUDEPATH += ../\
    ../../pluginutils \
    ../../../thirdparty/aosdk

SOURCES += \
    main.cpp \
    abstractparse.cpp \
    fileparse.cpp \
    zipparse.cpp \
    ../../pluginutils/pluginutils.cpp

HEADERS += \
    abstractparse.h \
    fileparse.h \
    zipparse.h \
    ../../pluginutils/pluginutils.h

win32 {
    DLLDESTDIR = Z:/build/foreplayer/debug/debug/plugins/backend/ao
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}