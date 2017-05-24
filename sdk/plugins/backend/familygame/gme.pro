#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT -= core gui

TARGET = plugin
TEMPLATE = lib
CONFIG += c++11

INCLUDEPATH += ../\
    ../../pluginutils \
    ../../../include

LIBS += -L$$PWD/../../../lib -lunrar -lgme -lunzip -lz

SOURCES += \
    main.cpp \
    gmewrap.cpp \
    rarparse.cpp \
    fileparse.cpp \
    abstractparse.cpp \
    zipparse.cpp \
    ../../pluginutils/pluginutils.cpp

HEADERS += \  
    gmewrap.h \
    rarparse.h \
    fileparse.h \
    abstractparse.h \
    zipparse.h \
    ../../pluginutils/pluginutils.h

win32 {
    DLLDESTDIR = $(FOREPLAYER_OUTPUT_PATH)/plugins/backend/gme
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
