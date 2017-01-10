#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T09:31:26
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = foreplayer
TEMPLATE = app

INCLUDEPATH += \
    utils

SOURCES += main.cpp

HEADERS  += \
    plugins/backend/backendinterface.h

include(core/core.pri)
include(gui/gui.pri)
include(controller/controller.pri)
include(utils/utils.pri)

CONFIG(debug, debug|release){
    TARGET = $$join(TARGET,,,d)
}
