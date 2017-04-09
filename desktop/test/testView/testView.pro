#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T15:02:58
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = testView
TEMPLATE = app

INCLUDEPATH += \
    ../../app/utils \
    ../../app/gui/windows/widgets \
    ../../app/gui/windows/playlist

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../app/gui/windows/playlist/playlistwidget.cpp \
    ../../app/gui/windows/playlist/view/abstracttableview.cpp \
    ../../app/gui/windows/playlist/view/musiclistview.cpp \
    ../../app/gui/windows/playlist/view/playlistview.cpp \
    ../../app/gui/windows/playlist/view/tracklistview.cpp \
    ../../app/gui/windows/playlist/model/abstractmodel.cpp \
    ../../app/gui/windows/playlist/model/musiclistmodel.cpp \
    ../../app/gui/windows/playlist/model/playlistmodel.cpp \
    ../../app/gui/windows/playlist/model/tracklistmodel.cpp \
    ../../app/gui/windows/widgets/buttonimage.cpp \
    ../../app/gui/windows/widgets/checkbox.cpp \
    ../../app/gui/windows/widgets/imagebutton.cpp \
    ../../app/gui/windows/widgets/label.cpp \
    ../../app/gui/windows/widgets/ledwidget.cpp \
    ../../app/gui/windows/widgets/linklabel.cpp \
    ../../app/gui/windows/widgets/scrollbar.cpp \
    ../../app/gui/windows/widgets/scrolllabel.cpp \
    ../../app/gui/windows/widgets/sliderbar.cpp \
    ../../app/gui/windows/widgets/splitter.cpp \
    ../../app/gui/windows/widgets/toolbar.cpp \
    ../../app/gui/windows/widgets/visualwidget.cpp \
    ../../app/gui/windows/widgets/style/abstractstyle.cpp \
    ../../app/gui/windows/widgets/style/sliderbarstyle.cpp \
    ../../app/gui/windows/widgets/style/scrollbarstyle.cpp \
    ../../app/utils/utils.cpp

HEADERS  += mainwindow.h \
    ../../app/gui/windows/playlist/playlistwidget.h \
    ../../app/gui/windows/playlist/view/abstracttableview.h \
    ../../app/gui/windows/playlist/view/musiclistview.h \
    ../../app/gui/windows/playlist/view/playlistview.h \
    ../../app/gui/windows/playlist/view/tracklistview.h \
    ../../app/gui/windows/playlist/model/abstractmodel.h \
    ../../app/gui/windows/playlist/model/musiclistmodel.h \
    ../../app/gui/windows/playlist/model/playlistmodel.h \
    ../../app/gui/windows/playlist/model/tracklistmodel.h \
    ../../app/gui/windows/widgets/buttonimage.h \
    ../../app/gui/windows/widgets/checkbox.h \
    ../../app/gui/windows/widgets/imagebutton.h \
    ../../app/gui/windows/widgets/label.h \
    ../../app/gui/windows/widgets/ledwidget.h \
    ../../app/gui/windows/widgets/linklabel.h \
    ../../app/gui/windows/widgets/scrollbar.h \
    ../../app/gui/windows/widgets/scrolllabel.h \
    ../../app/gui/windows/widgets/sliderbar.h \
    ../../app/gui/windows/widgets/splitter.h \
    ../../app/gui/windows/widgets/toolbar.h \
    ../../app/gui/windows/widgets/visualwidget.h \
    ../../app/gui/windows/widgets/style/abstractstyle.h \
    ../../app/gui/windows/widgets/style/sliderbarstyle.h \
    ../../app/gui/windows/widgets/style/scrollbarstyle.h \
    ../../app/utils/utils.h

FORMS    += mainwindow.ui
