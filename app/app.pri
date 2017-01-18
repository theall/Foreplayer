SOURCES += \
    $$PWD/app.cpp

HEADERS  += \
    $$PWD/plugins/backend/backendinterface.h \
    $$PWD/app.h

include($$PWD/core/core.pri)
include($$PWD/gui/gui.pri)
include($$PWD/controller/controller.pri)
include($$PWD/utils/utils.pri)
