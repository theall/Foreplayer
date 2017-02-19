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

SOURCES += \
    main.cpp

HEADERS +=

win32 {
    DLLDESTDIR = Z:/build/foreplayer/debug/debug/plugins/backend/ao
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
