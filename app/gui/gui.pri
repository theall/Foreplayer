QT += gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += \
    $$PWD/share

LIBS += -lunzip -lz

SOURCES += \
    $$PWD/windows/widgets/imagebutton.cpp \
    $$PWD/windows/widgets/buttonimage.cpp \
    $$PWD/windows/widgets/scrolllabel.cpp \
    $$PWD/windows/widgets/ledwidget.cpp \
    $$PWD/windows/widgets/label.cpp \
    $$PWD/windows/widgets/sliderbar.cpp \
    $$PWD/windows/widgets/visualwidget.cpp \
    $$PWD/windows/widgets/toolbar.cpp \
    $$PWD/windows/widgets/scrollbar.cpp \
    $$PWD/windows/widgets/linklabel.cpp \
    $$PWD/windows/widgets/checkbox.cpp \
    $$PWD/windows/widgets/splitter.cpp \
    $$PWD/windows/widgets/animationmenu.cpp \
    $$PWD/menus/mainmenu.cpp \
    $$PWD/menus/playcontrolmenu.cpp \
    $$PWD/menus/volumecontrolmenu.cpp \
    $$PWD/menus/playmodemenu.cpp \
    $$PWD/menus/skinmenu.cpp \
    $$PWD/menus/transparentmenu.cpp \
    $$PWD/menus/equalizermenu.cpp \
    $$PWD/windows/playlist/view/playlistview.cpp \
    $$PWD/windows/playlist/view/musiclistview.cpp \
    $$PWD/windows/playlist/view/tracklistview.cpp \
    $$PWD/windows/playlist/playlistwidget.cpp \
    $$PWD/menus/abstractmenu.cpp \
    $$PWD/guimanager.cpp \
    $$PWD/windows/abstractwindow.cpp \
    $$PWD/windows/browserwindow.cpp \
    $$PWD/windows/desctoplyricwindow.cpp \
    $$PWD/windows/desktopwindow.cpp \
    $$PWD/windows/equalizerwindow.cpp \
    $$PWD/windows/lyricwindow.cpp \
    $$PWD/windows/mainwindow.cpp \
    $$PWD/windows/playlistwindow.cpp \
    $$PWD/windows/playlist/view/abstracttableview.cpp \
    $$PWD/windows/widgets/style/sliderbarstyle.cpp \
    $$PWD/windows/widgets/style/scrollbarstyle.cpp \
    $$PWD/windows/widgets/style/abstractstyle.cpp \
    $$PWD/windows/playlist/menu/popmenuplaylist.cpp \
    $$PWD/windows/playlist/menu/popmenumusiclist.cpp \
    $$PWD/windows/playlist/menu/popmenutracklist.cpp \
    $$PWD/windows/playlist/menu/popmenuaddmusics.cpp \
    $$PWD/windows/playlist/menu/popmenuremovemusics.cpp \
    $$PWD/windows/playlist/menu/popmenusort.cpp \
    $$PWD/windows/playlist/menu/abstractpopmenu.cpp \
    $$PWD/windows/playlist/menu/popmenufind.cpp \
    $$PWD/windows/playlist/menu/popmenumusiclistitem.cpp \
    $$PWD/windows/playlist/menu/popmenuplaymode.cpp \
    $$PWD/windows/playlist/menu/popmenumusiclistedit.cpp \
    $$PWD/share/skin.cpp \
    $$PWD/share/skinutils.cpp \
    $$PWD/windows/miniwindow.cpp

HEADERS  += \
    $$PWD/windows/widgets/imagebutton.h \
    $$PWD/windows/widgets/buttonimage.h \
    $$PWD/windows/widgets/scrolllabel.h \
    $$PWD/windows/widgets/ledwidget.h \
    $$PWD/windows/widgets/label.h \
    $$PWD/windows/widgets/sliderbar.h \
    $$PWD/windows/widgets/visualwidget.h \
    $$PWD/windows/abstractwindow.h \
    $$PWD/windows/lyricwindow.h \
    $$PWD/windows/mainwindow.h \
    $$PWD/windows/equalizerwindow.h \
    $$PWD/windows/playlistwindow.h \
    $$PWD/windows/widgets/toolbar.h \
    $$PWD/windows/widgets/scrollbar.h \
    $$PWD/windows/browserwindow.h \
    $$PWD/windows/desctoplyricwindow.h \
    $$PWD/windows/widgets/linklabel.h \
    $$PWD/windows/widgets/checkbox.h \
    $$PWD/windows/desktopwindow.h \
    $$PWD/windows/widgets/splitter.h \
    $$PWD/windows/widgets/animationmenu.h \
    $$PWD/menus/mainmenu.h \
    $$PWD/menus/abstractmenu.h \
    $$PWD/menus/playcontrolmenu.h \
    $$PWD/menus/volumecontrolmenu.h \
    $$PWD/menus/playmodemenu.h \
    $$PWD/menus/skinmenu.h \
    $$PWD/menus/transparentmenu.h \
    $$PWD/menus/equalizermenu.h \
    $$PWD/windows/playlist/view/playlistview.h \
    $$PWD/windows/playlist/view/musiclistview.h \
    $$PWD/windows/playlist/view/tracklistview.h \
    $$PWD/windows/playlist/playlistwidget.h \
    $$PWD/guimanager.h \
    $$PWD/windows/playlist/view/abstracttableview.h \
    $$PWD/windows/widgets/style/sliderbarstyle.h \
    $$PWD/windows/widgets/style/scrollbarstyle.h \
    $$PWD/windows/widgets/style/abstractstyle.h \
    $$PWD/windows/playlist/menu/popmenuplaylist.h \
    $$PWD/windows/playlist/menu/popmenumusiclist.h \
    $$PWD/windows/playlist/menu/popmenutracklist.h \
    $$PWD/windows/playlist/menu/popmenuaddmusics.h \
    $$PWD/windows/playlist/menu/popmenuremovemusics.h \
    $$PWD/windows/playlist/menu/popmenusort.h \
    $$PWD/windows/playlist/menu/abstractpopmenu.h \
    $$PWD/windows/playlist/menu/popmenufind.h \
    $$PWD/windows/playlist/menu/popmenumusiclistitem.h \
    $$PWD/windows/playlist/menu/popmenuplaymode.h \
    $$PWD/windows/playlist/menu/popmenumusiclistedit.h \
    $$PWD/share/skin.h \
    $$PWD/share/skinutils.h \
    $$PWD/windows/miniwindow.h
