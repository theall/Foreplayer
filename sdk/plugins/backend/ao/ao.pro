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

LIBS += -L$$PWD/../../../lib -laosdk
LIBS += -lunzip -lz

INCLUDEPATH += ../\
    ../../pluginutils \
    ../../../include

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
    DLLDESTDIR = $(FOREPLAYER_OUTPUT_PATH)/plugins/backend/ao
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
