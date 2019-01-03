include($$PWD/legacy/legacy.pri)
#include($$PWD/uniform/uniform.pri)

SOURCES += \
    $$PWD/shared/model/playlistmodel.cpp \
    $$PWD/shared/model/tracklistmodel.cpp \
    $$PWD/shared/model/musiclistmodel.cpp \
    $$PWD/shared/model/abstractmodel.cpp \
    $$PWD/shared/model/missionsmodel.cpp \
    $$PWD/shared/model/pluginmodel.cpp \
    $$PWD/controllerproxy.cpp

HEADERS += \
    $$PWD/shared/model/playlistmodel.h \
    $$PWD/shared/model/tracklistmodel.h \
    $$PWD/shared/model/musiclistmodel.h \
    $$PWD/shared/model/abstractmodel.h \
    $$PWD/shared/model/missionsmodel.h \
    $$PWD/shared/model/pluginmodel.h \
    $$PWD/controllerproxy.h
