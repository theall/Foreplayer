#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT += core

TARGET = plugin
TEMPLATE = lib

DEFINES += 

LIBS += F:/opensource/myplayer/Mu-master/src/thirdpart/bass/lib/bass.dll

SOURCES += \
    main.cpp

HEADERS +=

win32 {
    DLLDESTDIR = Z:/build/foreplayer/debug/debug/plugins/backend/default
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
