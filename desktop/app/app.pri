SOURCES += \
    $$PWD/app.cpp \
    $$PWD/version/version.cpp

HEADERS  += \
    $$PWD/app.h \
    $$PWD/version/version.h

include($$PWD/core/core.pri)
include($$PWD/gui/gui.pri)
include($$PWD/controller/controller.pri)
include($$PWD/utils/utils.pri)

win32:{
    RC_FILE = $$PWD/version/res.rc
    HEADERS += $$PWD/version/win32_ver.h
    LIBS += -lversion
}
