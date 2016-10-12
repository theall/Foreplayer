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
    utils

SOURCES += main.cpp\
    gui/skinloader/skinloader.cpp \
    utils/utils.cpp \
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
    gui/menus/tplaycontrolmenu.cpp \
    gui/menus/tvolumecontrolmenu.cpp \
    gui/menus/tplaymodemenu.cpp \
    gui/menus/tskinmenu.cpp \
    gui/menus/ttransparentmenu.cpp \
    gui/menus/tequalizermenu.cpp \
    core/filter/tsamplesfilter.cpp \
    controller/model/playlistmodel.cpp \
    controller/model/tracklistmodel.cpp \
    controller/model/musiclistmodel.cpp \
    gui/windows/playlist/playlistview.cpp \
    gui/windows/playlist/musiclistview.cpp \
    gui/windows/playlist/tracklistview.cpp \
    gui/windows/playlist/playlistwidget.cpp \
    controller/playlistcontroller.cpp \
    core/core.cpp \
    controller/controller.cpp \
    core/playercore.cpp \
    core/playlistcore.cpp \
    controller/backendcontroller.cpp \
    core/front/directsoundfront.cpp \
    core/front/sdlfront.cpp \
    gui/menus/abstractmenu.cpp \
    core/backend/backendplugin.cpp \
    core/front/abstractfront.cpp \
    core/pluginmanager/abstractpluginmanager.cpp \
    core/pluginmanager/backendpluginmanager.cpp \
    controller/playercontroller.cpp \
    core/playthread.cpp

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
    gui/menus/tequalizermenu.h \
    core/filter/tsamplesfilter.h \
    controller/model/playlistmodel.h \
    controller/model/tracklistmodel.h \
    controller/model/musiclistmodel.h \
    gui/windows/playlist/playlistview.h \
    gui/windows/playlist/musiclistview.h \
    gui/windows/playlist/tracklistview.h \
    gui/windows/playlist/playlistwidget.h \
    controller/playlistcontroller.h \
    core/core.h \
    core/playercore.h \
    controller/controller.h \
    core/playlistcore.h \
    controller/backendcontroller.h \
    core/front/directsoundfront.h \
    core/front/sdlfront.h \
    core/front/abstractfront.h \
    core/backend/backendplugin.h \
    core/pluginmanager/abstractpluginmanager.h \
    core/pluginmanager/backendpluginmanager.h \
    plugins/backend/backendinterface.h \
    controller/playercontroller.h \
    core/playthread.h
