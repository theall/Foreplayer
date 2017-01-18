#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT += core

TARGET = plugin
TEMPLATE = lib
CONFIG += qt c++11

LIBS += -lgme -lunrar

SOURCES += \
    main.cpp \
    gmewrap.cpp \
    rarparse.cpp \
    fileparse.cpp \
    abstractparse.cpp

HEADERS += \  
    gmewrap.h \
    rarparse.h \
    fileparse.h \
    abstractparse.h

win32 {
    DLLDESTDIR = Z:/build/foreplayer/debug/debug/plugins/backend/gme
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
