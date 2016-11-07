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

LIBS += -ldsound

SOURCES += main.cpp\
    gui/skinloader/skinloader.cpp \
    utils/utils.cpp \
    gui/windows/widgets/imagebutton.cpp \
    gui/windows/widgets/buttonimage.cpp \
    gui/windows/widgets/scrolllabel.cpp \
    gui/windows/widgets/ledwidget.cpp \
    gui/windows/widgets/label.cpp \
    gui/windows/widgets/sliderbar.cpp \
    gui/windows/widgets/visualwidget.cpp \
    gui/skinutils.cpp \
    gui/windows/widgets/toolbar.cpp \
    gui/windows/widgets/scrollbar.cpp \
    gui/windows/widgets/linklabel.cpp \
    gui/windows/widgets/checkbox.cpp \
    gui/windows/widgets/splitter.cpp \
    gui/windows/widgets/animationmenu.cpp \
    gui/menus/mainmenu.cpp \
    gui/menus/playcontrolmenu.cpp \
    gui/menus/volumecontrolmenu.cpp \
    gui/menus/playmodemenu.cpp \
    gui/menus/skinmenu.cpp \
    gui/menus/transparentmenu.cpp \
    gui/menus/equalizermenu.cpp \
    core/filter/samplesfilter.cpp \
    gui/windows/playlist/model/playlistmodel.cpp \
    gui/windows/playlist/model/tracklistmodel.cpp \
    gui/windows/playlist/model/musiclistmodel.cpp \
    gui/windows/playlist/view/playlistview.cpp \
    gui/windows/playlist/view/musiclistview.cpp \
    gui/windows/playlist/view/tracklistview.cpp \
    gui/windows/playlist/playlistwidget.cpp \
    controller/playlistcontroller.cpp \
    core/core.cpp \
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
    core/playthread.cpp \
    controller/abstractcontroller.cpp \
    gui/guimanager.cpp \
    gui/windows/abstractwindow.cpp \
    gui/windows/browserwindow.cpp \
    gui/windows/desctoplyricwindow.cpp \
    gui/windows/desktopwindow.cpp \
    gui/windows/equalizerwindow.cpp \
    gui/windows/lyricwindow.cpp \
    gui/windows/mainwindow.cpp \
    gui/windows/playlistwindow.cpp \
    controller/maincontroller.cpp \
    gui/windows/playlist/model/abstractmodel.cpp \
    gui/windows/playlist/view/abstracttableview.cpp \
    gui/windows/widgets/style/sliderbarstyle.cpp \
    gui/windows/widgets/style/scrollbarstyle.cpp \
    gui/windows/widgets/style/abstractstyle.cpp

HEADERS  += \
    utils/pch.h \
    gui/windows/widgets/imagebutton.h \
    gui/windows/widgets/buttonimage.h \
    gui/windows/widgets/scrolllabel.h \
    gui/windows/widgets/ledwidget.h \
    gui/windows/widgets/label.h \
    gui/windows/widgets/sliderbar.h \
    gui/windows/widgets/visualwidget.h \
    gui/windows/abstractwindow.h \
    gui/windows/lyricwindow.h \
    gui/skinloader/skinloader.h \
    gui/windows/mainwindow.h \
    gui/windows/equalizerwindow.h \
    gui/windows/playlistwindow.h \
    gui/skinutils.h \
    utils/utils.h \
    gui/windows/widgets/toolbar.h \
    gui/windows/widgets/scrollbar.h \
    gui/windows/browserwindow.h \
    gui/windows/desctoplyricwindow.h \
    gui/windows/widgets/linklabel.h \
    gui/windows/widgets/checkbox.h \
    gui/windows/desktopwindow.h \
    gui/windows/widgets/splitter.h \
    gui/windows/widgets/animationmenu.h \
    gui/menus/mainmenu.h \
    gui/menus/abstractmenu.h \
    gui/menus/playcontrolmenu.h \
    gui/menus/volumecontrolmenu.h \
    gui/menus/playmodemenu.h \
    gui/menus/skinmenu.h \
    gui/menus/transparentmenu.h \
    gui/menus/equalizermenu.h \
    core/filter/samplesfilter.h \
    gui/windows/playlist/model/playlistmodel.h \
    gui/windows/playlist/model/tracklistmodel.h \
    gui/windows/playlist/model/musiclistmodel.h \
    gui/windows/playlist/view/playlistview.h \
    gui/windows/playlist/view/musiclistview.h \
    gui/windows/playlist/view/tracklistview.h \
    gui/windows/playlist/playlistwidget.h \
    controller/playlistcontroller.h \
    core/core.h \
    core/playercore.h \
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
    core/playthread.h \
    controller/abstractcontroller.h \
    gui/guimanager.h \
    controller/maincontroller.h \
    gui/windows/playlist/model/abstractmodel.h \
    gui/windows/playlist/view/abstracttableview.h \
    gui/windows/widgets/style/sliderbarstyle.h \
    gui/windows/widgets/style/scrollbarstyle.h \
    gui/windows/widgets/style/abstractstyle.h
