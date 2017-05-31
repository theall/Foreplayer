QT += core gui widgets xml

TARGET = exportor
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -lmp3lame

SOURCES += main.cpp \
    export/abstractexport.cpp \
    export/waveexport.cpp \
    export/mp3export.cpp \
    export/exportfactory.cpp \
    commandline/cmdlineparser.cpp \
    export/pcmexport.cpp

include(../../app/core/core.pri)
include(../../app/utils/utils.pri)

HEADERS += \
    export/abstractexport.h \
    export/waveexport.h \
    export/mp3export.h \
    export/exportfactory.h \
    commandline/cmdlineparser.h \
    export/pcmexport.h

CONFIG(debug, debug|release){
    CONFIG += console
    DESTDIR = $(FOREPLAYER_OUTPUT_PATH)
} else {
}
