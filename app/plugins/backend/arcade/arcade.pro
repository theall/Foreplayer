#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T15:03:11
#
#-------------------------------------------------

QT += core

TARGET = arcade
TEMPLATE = lib

DEFINES += 

SOURCES += \
    main.cpp \
    m1thread.cpp

HEADERS += \  
    m1thread.h \
    utils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
