#ifndef TGUIMANAGER_H
#define TGUIMANAGER_H

#include "pch.h"

#include "windows/mainwindow.h"
#include "windows/miniwindow.h"
#include "windows/lyricwindow.h"
#include "windows/equalizerwindow.h"
#include "windows/playlistwindow.h"
#include "windows/browserwindow.h"
#include "windows/desctoplyricwindow.h"
#include "windows/desktopwindow.h"

#include "menus/mainmenu.h"

#include "share/skin.h"

typedef QList<TAbstractWindow *> TWindowList;

class TGuiManager : public QObject
{
    Q_OBJECT
public:
    explicit TGuiManager(QObject *parent = 0);
    ~TGuiManager();

    void loadSkin(QString skinPath);

    void showGui();

    TLyricWindow *lyricWindow() { return mLyricWindow; }
    TMainWindow *mainWindow() { return mMainWindow; }
    TEqualizerWindow *equalizerWindow() { return mEqualizerWindow; }
    TPlaylistWindow *playlistWindow() { return mPlaylistWindow; }
    TBrowserWindow *browserWindow() { return mBrowserWindow; }
    TDesktopLyricWindow *desktopLyricWindow() { return mDesktopLyricWindow; }

signals:
    void requestPlay();
    void requestPause();
    void requestStop();
    void requestNext();
    void requestPrevious();

private slots:
    void slotLyricWindowToggled(bool toggled);
    void slotLyricButtonToggled(bool toggled);
    void slotEqualizerWindowToggled(bool toggled);
    void slotEqualizerButtonToggled(bool toggled);
    void slotPlaylistWindowToggled(bool toggled);
    void slotPlaylistButtonToggled(bool toggled);
    void slotBrowserWindowToggled(bool toggled);
    void slotBrowserButtonToggled(bool toggled);
    void slotRequestMoveWindow(QPoint newPos);
    void slotRequestResizeWindow(QRect newGeometry);
    void slotRequireShowLyricWindow();
    void slotRequireShowDesktopLyric();
    void slotShowMinimized();
    void slotRequestExit();
    void slotRequestToggleWindow();
    void slotRequestRestoreWindow();
    void slotExit();
    void slotOnOpacityChanged(qreal value);
    void slotRequestLoadSkin(QString skinFullName);
    void slotMainWindowActivationChanged();
    void slotMainWindowAboutToClose();

private:
    TMainWindow *mMainWindow;
    TMiniWindow *mMiniWindow;
    TLyricWindow *mLyricWindow;
    TEqualizerWindow *mEqualizerWindow;
    TPlaylistWindow *mPlaylistWindow;
    TBrowserWindow *mBrowserWindow;
    TDesktopLyricWindow *mDesktopLyricWindow;
    TDesktopWindow *mDesktopWindow;
    TWindowList mGumedWindows;

    TMainMenu *mMainMenu;
    bool mMinimode;
    bool mShowDesktopLyric;
    bool mBrowserVisible;
    bool mPlaylistVisible;
    bool mLyricWindowVisible;
    bool mDesktopLyricVisible;
    bool mEqualizerVisible;

    void createSystemTrayIcon();
    void hide();
    void show();
    void moveWindow(TAbstractWindow* window, int left, int top);
};

#endif // TGUIMANAGER_H
