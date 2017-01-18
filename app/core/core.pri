QT += core
LIBS += -ldsound -lsdl2

SOURCES += \
    $$PWD/filter/samplesfilter.cpp \
    $$PWD/core.cpp \
    $$PWD/playercore.cpp \
    $$PWD/playlistcore.cpp \
    $$PWD/front/directsoundfront.cpp \
    $$PWD/front/sdlfront.cpp \
    $$PWD/pluginmanager/backend/backendplugin.cpp \
    $$PWD/front/abstractfront.cpp \
    $$PWD/pluginmanager/abstractpluginmanager.cpp \
    $$PWD/pluginmanager/backendpluginmanager.cpp \
    $$PWD/playthread.cpp \
    $$PWD/playlist/playlistitem.cpp \
    $$PWD/playlist/musicitem.cpp \
    $$PWD/playlist/trackitem.cpp

HEADERS  += \
    $$PWD/filter/samplesfilter.h \
    $$PWD/core.h \
    $$PWD/playercore.h \
    $$PWD/playlistcore.h \
    $$PWD/front/directsoundfront.h \
    $$PWD/front/sdlfront.h \
    $$PWD/front/abstractfront.h \
    $$PWD/pluginmanager/backend/backendplugin.h \
    $$PWD/pluginmanager/abstractpluginmanager.h \
    $$PWD/pluginmanager/backendpluginmanager.h \
    $$PWD/playthread.h \
    $$PWD/playlist/playlistitem.h \
    $$PWD/playlist/musicitem.h \
    $$PWD/playlist/trackitem.h \
    $$PWD/playlist/playlistdef.h
