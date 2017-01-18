#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT -= core gui

TARGET = plugin
TEMPLATE = lib
CONFIG += qt c++11

DEFINES += 

SOURCES += \
    main.cpp

HEADERS +=   

unix {
    target.path = /usr/lib
    INSTALLS += target
}
