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
    F:/opensource/foreplayer/utils \
    F:/opensource/foreplayer/gui/windows/widgets \
    F:/opensource/foreplayer/gui/windows/playlist

SOURCES += main.cpp\
        mainwindow.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/playlistwidget.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/view/abstracttableview.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/view/musiclistview.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/view/playlistview.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/view/tracklistview.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/model/abstractmodel.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/model/musiclistmodel.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/model/playlistmodel.cpp \
    F:/opensource/foreplayer/gui/windows/playlist/model/tracklistmodel.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/buttonimage.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/checkbox.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/imagebutton.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/label.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/ledwidget.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/linklabel.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/scrollbar.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/scrolllabel.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/sliderbar.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/splitter.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/toolbar.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/visualwidget.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/style/abstractstyle.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/style/sliderbarstyle.cpp \
    F:/opensource/foreplayer/gui/windows/widgets/style/scrollbarstyle.cpp \
    F:/opensource/foreplayer/utils/utils.cpp

HEADERS  += mainwindow.h \
    F:/opensource/foreplayer/gui/windows/playlist/playlistwidget.h \
    F:/opensource/foreplayer/gui/windows/playlist/view/abstracttableview.h \
    F:/opensource/foreplayer/gui/windows/playlist/view/musiclistview.h \
    F:/opensource/foreplayer/gui/windows/playlist/view/playlistview.h \
    F:/opensource/foreplayer/gui/windows/playlist/view/tracklistview.h \
    F:/opensource/foreplayer/gui/windows/playlist/model/abstractmodel.h \
    F:/opensource/foreplayer/gui/windows/playlist/model/musiclistmodel.h \
    F:/opensource/foreplayer/gui/windows/playlist/model/playlistmodel.h \
    F:/opensource/foreplayer/gui/windows/playlist/model/tracklistmodel.h \
    F:/opensource/foreplayer/gui/windows/widgets/buttonimage.h \
    F:/opensource/foreplayer/gui/windows/widgets/checkbox.h \
    F:/opensource/foreplayer/gui/windows/widgets/imagebutton.h \
    F:/opensource/foreplayer/gui/windows/widgets/label.h \
    F:/opensource/foreplayer/gui/windows/widgets/ledwidget.h \
    F:/opensource/foreplayer/gui/windows/widgets/linklabel.h \
    F:/opensource/foreplayer/gui/windows/widgets/scrollbar.h \
    F:/opensource/foreplayer/gui/windows/widgets/scrolllabel.h \
    F:/opensource/foreplayer/gui/windows/widgets/sliderbar.h \
    F:/opensource/foreplayer/gui/windows/widgets/splitter.h \
    F:/opensource/foreplayer/gui/windows/widgets/toolbar.h \
    F:/opensource/foreplayer/gui/windows/widgets/visualwidget.h \
    F:/opensource/foreplayer/gui/windows/widgets/style/abstractstyle.h \
    F:/opensource/foreplayer/gui/windows/widgets/style/sliderbarstyle.h \
    F:/opensource/foreplayer/gui/windows/widgets/style/scrollbarstyle.h \
    F:/opensource/foreplayer/utils/utils.h

FORMS    += mainwindow.ui
