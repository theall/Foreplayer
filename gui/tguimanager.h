#ifndef TGUIMANAGER_H
#define TGUIMANAGER_H

#include "pch.h"
#include "windows/tmainwindow.h"
#include "windows/tlyricwindow.h"
#include "windows/tequalizerwindow.h"
#include "windows/tplaylistwindow.h"
#include "windows/tbrowserwindow.h"
#include "windows/tdesctoplyricwindow.h"
#include "windows/tdesktopwindow.h"

#include "menus/tmainmenu.h"

typedef QList<TAbstractWindow *> TWindowList;

class TGuiManager : public QObject
{
    Q_OBJECT
public:
    explicit TGuiManager(QObject *parent = 0);

    void loadSkin(QString skinPath);

    void showGui();

    TLyricWindow *lyricWindow();
    TMainWindow *mainWindow();
    TEqualizerWindow *equalizerWindow();
    TPlaylistWindow *playlistWindow();
    TBrowserWindow *browserWindow();
    TDesktopLyricWindow *desktopLyricWindow();

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
    void slotRequestToggleWindow();
    void slotRequestRestoreWindow();
    void slotExit();

private:
    TMainWindow *mMainWindow;
    TLyricWindow *mLyricWindow;
    TEqualizerWindow *mEqualizerWindow;
    TPlaylistWindow *mPlaylistWindow;
    TBrowserWindow *mBrowserWindow;
    TDesktopLyricWindow *mDesktopLyricWindow;
    TDesktopWindow *mDesktopWindow;
    TWindowList mGumedWindows;

    TMainMenu *mMainMenu;
    bool mShowDesktopLyric;
    bool mBrowserVisiable;
    bool mPlaylistVisiable;
    bool mLyricWindowVisiable;
    bool mDesktopLyricVisiable;
    bool mEqualizerVisiable;

    void createSystemTrayIcon();
    void hide();
    void show();
    void moveWindow(TAbstractWindow* window, int left, int top);
};

#endif // TGUIMANAGER_H
