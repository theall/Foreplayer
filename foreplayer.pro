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
    utils \
    core \
    gui \
    gui/skinloader \
    gui/windows \
    gui/windows/widgets \
    controller

SOURCES += main.cpp\
    gui/skinloader/skinloader.cpp \
    utils/utils.cpp \
    controller/tcontroller.cpp \
    gui/windows/widgets/timagebutton.cpp \
    gui/windows/widgets/tbuttonimage.cpp \
    gui/windows/widgets/tscrolllabel.cpp \
    gui/windows/widgets/tledwidget.cpp \
    gui/windows/widgets/tlabel.cpp \
    gui/windows/widgets/tsliderbar.cpp \
    gui/windows/widgets/tsliderstyle.cpp \
    gui/windows/widgets/tvisualwidget.cpp \
    gui/windows/tabstractwindow.cpp \
    gui/windows/tlyricwindow.cpp \
    gui/tguimanager.cpp \
    core/tplayercore.cpp \
    gui/windows/tmainwindow.cpp \
    gui/windows/tequalizerwindow.cpp \
    gui/windows/tplaylistwindow.cpp \
    gui/skinutils.cpp \
    gui/windows/widgets/ttoolbar.cpp \
    gui/windows/widgets/tscrollbar.cpp \
    gui/windows/tbrowserwindow.cpp \
    gui/windows/tdesctoplyricwindow.cpp \
    gui/windows/widgets/tlinklabel.cpp \
    gui/windows/widgets/tcheckbox.cpp \
    gui/windows/tdesktopwindow.cpp \
    gui/windows/widgets/tsplitter.cpp \
    gui/windows/widgets/tanimationmenu.cpp \
    gui/menus/tmainmenu.cpp \
    gui/menus/tabstractmenu.cpp \
    gui/menus/tplaycontrolmenu.cpp \
    gui/menus/tvolumecontrolmenu.cpp \
    gui/menus/tplaymodemenu.cpp \
    gui/menus/tskinmenu.cpp \
    gui/menus/ttransparentmenu.cpp \
    gui/menus/tequalizermenu.cpp

HEADERS  += \
    utils/pch.h \
    gui/windows/widgets/timagebutton.h \
    gui/windows/widgets/tbuttonimage.h \
    gui/windows/widgets/tscrolllabel.h \
    gui/windows/widgets/tledwidget.h \
    gui/windows/widgets/tlabel.h \
    gui/windows/widgets/tsliderbar.h \
    gui/windows/widgets/tsliderstyle.h \
    gui/windows/widgets/tvisualwidget.h \
    gui/windows/tabstractwindow.h \
    gui/windows/tlyricwindow.h \
    gui/tguimanager.h \
    gui/skinloader/skinloader.h \
    controller/tcontroller.h \
    core/tplayercore.h \
    gui/windows/tmainwindow.h \
    gui/windows/tequalizerwindow.h \
    gui/windows/tplaylistwindow.h \
    gui/skinutils.h \
    utils/utils.h \
    gui/windows/widgets/ttoolbar.h \
    gui/windows/widgets/tscrollbar.h \
    gui/windows/tbrowserwindow.h \
    gui/windows/tdesctoplyricwindow.h \
    gui/windows/widgets/tlinklabel.h \
    gui/windows/widgets/tcheckbox.h \
    gui/windows/tdesktopwindow.h \
    gui/windows/widgets/tsplitter.h \
    gui/windows/widgets/tanimationmenu.h \
    gui/menus/tmainmenu.h \
    gui/menus/tabstractmenu.h \
    gui/menus/tplaycontrolmenu.h \
    gui/menus/tvolumecontrolmenu.h \
    gui/menus/tplaymodemenu.h \
    gui/menus/tskinmenu.h \
    gui/menus/ttransparentmenu.h \
    gui/menus/tequalizermenu.h
