#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T09:31:26
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = testCore
TEMPLATE = app

INCLUDEPATH += \
    ../../utils \
    ../../core

LIBS += -ldsound

SOURCES += \
    main.cpp
    
HEADERS += 

include(../../core/core.pri)
