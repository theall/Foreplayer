#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T12:14:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = unzip
TEMPLATE = lib

DEFINES +=

LIBS += -lzlib

SOURCES += unzip.c

HEADERS += unzip.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
