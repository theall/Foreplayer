#-------------------------------------------------
#
# Project created by QtCreator 2017-04-04T16:18:50
#
#-------------------------------------------------

QT       -= core

TARGET = foreplayer
TEMPLATE = lib
CONFIG += c++11

DEFINES += FOREPLAYER_EXPORT

INCLUDEPATH += \
    include \
    thirdparty

LIBS += -L$$PWD/lib -lfftreal -laudio3d
LIBS += -lsdl2 -lz

DESTDIR = $(FOREPLAYER_OUTPUT_PATH)

SOURCES += \
    $$PWD/src/filter/samplesfilter.cpp \
    $$PWD/src/front/abstractfront.cpp \
    $$PWD/src/front/loopbuffer.cpp \
    $$PWD/src/front/sdlfront.cpp \
    $$PWD/src/playlist/musicitem.cpp \
    $$PWD/src/playlist/playlistitem.cpp \
    $$PWD/src/playlist/trackitem.cpp \
    $$PWD/src/pluginmanager/backend/backendplugin.cpp \
    $$PWD/src/pluginmanager/abstractpluginmanager.cpp \
    $$PWD/src/pluginmanager/backendpluginmanager.cpp \
    $$PWD/src/core.cpp \
    $$PWD/src/foreplayerlib.cpp \
    $$PWD/src/playercore.cpp \
    $$PWD/src/playlistcore.cpp \
    $$PWD/src/playthread.cpp \
    $$PWD/src/cplusutils.cpp \
    $$PWD/src/pluginmanager/library.cpp \
    $$PWD/src/playlist/abstractitem.cpp \
    $$PWD/src/exportcore.cpp

HEADERS += \
    include/foreplayer.h \
    $$PWD/src/filter/samplesfilter.h \
    $$PWD/src/front/abstractfront.h \
    $$PWD/src/front/loopbuffer.h \
    $$PWD/src/front/sdlfront.h \
    $$PWD/src/playlist/musicitem.h \
    $$PWD/src/playlist/playlistdef.h \
    $$PWD/src/playlist/playlistitem.h \
    $$PWD/src/playlist/trackitem.h \
    $$PWD/src/pluginmanager/backend/backendplugin.h \
    $$PWD/src/pluginmanager/abstractpluginmanager.h \
    $$PWD/src/pluginmanager/backendpluginmanager.h \
    $$PWD/src/core.h \
    $$PWD/src/playercore.h \
    $$PWD/src/playlistcore.h \
    $$PWD/src/playthread.h \
    $$PWD/src/strformat.hpp \
    $$PWD/src/cplusutils.h \
    $$PWD/src/pluginmanager/library.h \
    $$PWD/src/playlist/abstractitem.h \
    $$PWD/src/exportcore.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
