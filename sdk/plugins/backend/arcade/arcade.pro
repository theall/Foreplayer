#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT += core

TARGET = plugin
TEMPLATE = lib

DEFINES += 

LIBS += -LZ:/build/foreplayer/thirdparty/build-m1lib-Desktop_Qt_5_5_1_MinGW_32bit-debug/debug -lm1

SOURCES += \
    main.cpp \
    m1thread.cpp

HEADERS += \  
    m1thread.h

win32 {
    DLLDESTDIR = Z:/build/foreplayer/debug/debug/plugins/backend/arcade
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
