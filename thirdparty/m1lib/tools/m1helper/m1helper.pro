#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T17:22:16
#
#-------------------------------------------------

QT       -= core gui

TEMPLATE = app

CONFIG += console

DEFINES += M1

LIBS += -LZ:/build/foreplayer/thirdparty/build-m1lib-Desktop_Qt_5_5_1_MinGW_32bit-Debug/Debug -lm1

INCLUDEPATH += ../

CONFIG(debug, debug|release) {
    DESTDIR = Z:/build/foreplayer/debug/debug/plugins/backend/arcade
    TARGET = m1

} else {
    TARGET = m1
}

SOURCES += m1cui.c

HEADERS +=

windows {
    #DEFINES += WINAPI
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
