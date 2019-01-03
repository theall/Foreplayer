QT += gui widgets xml

INCLUDEPATH += \
    $$PWD/skin

LIBS += -lminizip -lbz2 -lz

SOURCES += \
    $$PWD/widgets/imagebutton.cpp \
    $$PWD/widgets/buttonimage.cpp \
    $$PWD/widgets/scrolllabel.cpp \
    $$PWD/widgets/ledwidget.cpp \
    $$PWD/widgets/label.cpp \
    $$PWD/widgets/sliderbar.cpp \
    $$PWD/widgets/visualwidget.cpp \
    $$PWD/widgets/toolbar.cpp \
    $$PWD/widgets/scrollbar.cpp \
    $$PWD/widgets/linklabel.cpp \
    $$PWD/widgets/checkbox.cpp \
    $$PWD/widgets/splitter.cpp \
    $$PWD/widgets/animationmenu.cpp \
    $$PWD/widgets/style/sliderbarstyle.cpp \
    $$PWD/widgets/style/scrollbarstyle.cpp \
    $$PWD/widgets/style/abstractstyle.cpp \
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
    $$PWD/windows/miniwindow.cpp \
    $$PWD/windows/playlistwindow.cpp \
    $$PWD/windows/playlist/view/abstracttableview.cpp \
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
    $$PWD/dialogs/abstractdialog.cpp \
    $$PWD/dialogs/exportdialog.cpp \
    $$PWD/dialogs/exportmissionsdialog.cpp \
    $$PWD/dialogs/optionsdialog.cpp \
    $$PWD/dialogs/propertydialog.cpp \
    $$PWD/dialogs/optionabout.cpp \
    $$PWD/dialogs/optionskin.cpp \
    $$PWD/dialogs/optionplugininfo.cpp \
    $$PWD/dialogs/optiongeneral.cpp \
    $$PWD/dialogs/modules/languagemanager.cpp \
    $$PWD/dialogs/optionpagebase.cpp \
    $$PWD/skin/skin.cpp \
    $$PWD/skin/skinsetting.cpp \
    $$PWD/skin/skinmanager.cpp \
    $$PWD/skin/skinutils.cpp

HEADERS  += \
    $$PWD/widgets/imagebutton.h \
    $$PWD/widgets/buttonimage.h \
    $$PWD/widgets/scrolllabel.h \
    $$PWD/widgets/ledwidget.h \
    $$PWD/widgets/label.h \
    $$PWD/widgets/sliderbar.h \
    $$PWD/widgets/visualwidget.h \
    $$PWD/widgets/toolbar.h \
    $$PWD/widgets/scrollbar.h \
    $$PWD/widgets/splitter.h \
    $$PWD/widgets/animationmenu.h \
    $$PWD/widgets/style/sliderbarstyle.h \
    $$PWD/widgets/style/scrollbarstyle.h \
    $$PWD/widgets/style/abstractstyle.h \
    $$PWD/widgets/linklabel.h \
    $$PWD/widgets/checkbox.h \
    $$PWD/windows/abstractwindow.h \
    $$PWD/windows/lyricwindow.h \
    $$PWD/windows/mainwindow.h \
    $$PWD/windows/miniwindow.h \
    $$PWD/windows/equalizerwindow.h \
    $$PWD/windows/playlistwindow.h \
    $$PWD/windows/browserwindow.h \
    $$PWD/windows/desctoplyricwindow.h \
    $$PWD/windows/desktopwindow.h \
    $$PWD/dialogs/abstractdialog.h \
    $$PWD/dialogs/exportdialog.h \
    $$PWD/dialogs/exportmissionsdialog.h \
    $$PWD/dialogs/optionsdialog.h \
    $$PWD/dialogs/propertydialog.h \
    $$PWD/dialogs/optionabout.h \
    $$PWD/dialogs/optionskin.h \
    $$PWD/dialogs/optionplugininfo.h \
    $$PWD/dialogs/optiongeneral.h \
    $$PWD/dialogs/modules/languagemanager.h \
    $$PWD/dialogs/optionpagebase.h \
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
    $$PWD/skin/skinsetting.h \
    $$PWD/skin/skinmanager.h \
    $$PWD/skin/skin.h \
    $$PWD/skin/skinutils.h

FORMS += \
    $$PWD/dialogs/optionsdialog.ui \
    $$PWD/dialogs/exportdialog.ui \
    $$PWD/dialogs/exportmissionsdialog.ui \
    $$PWD/dialogs/propertydialog.ui \
    $$PWD/dialogs/optiongeneral.ui \
    $$PWD/dialogs/optionabout.ui \
    $$PWD/dialogs/optionskin.ui \
    $$PWD/dialogs/optionplugininfo.ui
