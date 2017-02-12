QT += core gui widgets xml

TARGET = exportor
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    export/abstractexport.cpp \
    export/waveexport.cpp

include(../../app/core/core.pri)
include(../../app/utils/utils.pri)

HEADERS += \
    export/abstractexport.h \
    export/waveexport.h

CONFIG(debug, debug|release){
    DESTDIR = z:/build/foreplayer/debug/debug
}
