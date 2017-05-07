#-------------------------------------------------
#
# Project created by QtCreator 2017-04-04T16:18:50
#
#-------------------------------------------------

QT       -= core gui

TARGET = foreplayerlib
TEMPLATE = lib
CONFIG += c++11

DEFINES += FOREPLAYER_EXPORT

INCLUDEPATH += \
    include \
    thirdparty \
    D:/SDL2-2.0.4/i686-w64-mingw32/include

LIBS += -LF:/opensource/foreplayer/dist/lib -laudio3d
LIBS += -LD:/SDL2-2.0.4/i686-w64-mingw32/lib -lsdl2
LIBS += -LZ:/cmake/fft -lfftreal
LIBS += -lz

DESTDIR = Z:/build/foreplayer/debug/debug

SOURCES += \
    src/filter/samplesfilter.cpp \
    src/front/abstractfront.cpp \
    src/front/loopbuffer.cpp \
    src/front/sdlfront.cpp \
    src/playlist/musicitem.cpp \
    src/playlist/playlistitem.cpp \
    src/playlist/trackitem.cpp \
    src/pluginmanager/backend/backendplugin.cpp \
    src/pluginmanager/abstractpluginmanager.cpp \
    src/pluginmanager/backendpluginmanager.cpp \
    src/core.cpp \
    src/foreplayerlib.cpp \
    src/playercore.cpp \
    src/playlistcore.cpp \
    src/playthread.cpp \
    src/cplusutils.cpp \
    src/pluginmanager/library.cpp \
    src/playlist/abstractitem.cpp \
    src/exportcore.cpp

HEADERS += \
    include/foreplayer.h \
    src/filter/samplesfilter.h \
    src/front/abstractfront.h \
    src/front/loopbuffer.h \
    src/front/sdlfront.h \
    src/playlist/musicitem.h \
    src/playlist/playlistdef.h \
    src/playlist/playlistitem.h \
    src/playlist/trackitem.h \
    src/pluginmanager/backend/backendplugin.h \
    src/pluginmanager/abstractpluginmanager.h \
    src/pluginmanager/backendpluginmanager.h \
    src/core.h \
    src/playercore.h \
    src/playlistcore.h \
    src/playthread.h \
    src/strformat.hpp \
    src/cplusutils.h \
    src/pluginmanager/library.h \
    src/playlist/abstractitem.h \
    src/exportcore.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
