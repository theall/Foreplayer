#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T20:01:42
#
#-------------------------------------------------

QT       += core gui widgets xml testlib

TARGET = tst_unittesttest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += \
    ../../app/ \
    ../../app/utils

include(../../app/core/core.pri)
include(../../app/utils/utils.pri)

HEADERS += \
    app/utils/tst_utils.h

SOURCES += \
    main.cpp \
    app/utils/tst_utils.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
