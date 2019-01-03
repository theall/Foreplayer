QT += gui widgets
include($$PWD/legacy/legacy.pri)
#include($$PWD/uniform/uniform.pri)

INCLUDEPATH += \
    $$PWD

TRANSLATIONS = \
    $$PWD/resource/ts/en.ts \
    $$PWD/resource/ts/zh_cn.ts \
    $$PWD/resource/ts/zh_tw.ts

SOURCES += \
    $$PWD/abstractguimanager.cpp \
    $$PWD/guiproxy.cpp

HEADERS  += \
    $$PWD/abstractguimanager.h \
    $$PWD/guiproxy.h

RESOURCES += \
    $$PWD/resource/res.qrc
