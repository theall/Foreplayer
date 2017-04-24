/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
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

#include "dialogs/exportdialog.h"
#include "dialogs/exportmissionsdialog.h"
#include "dialogs/optionsdialog.h"
#include "dialogs/propertydialog.h"

#include "menus/mainmenu.h"

#include "share/skinmanager.h"

typedef QList<TAbstractWindow *> TWindowList;

class TGuiManager : public QObject
{
    Q_OBJECT

public:
    explicit TGuiManager(QObject *parent = 0);
    ~TGuiManager();

    bool loadSkin(QString skinPath = QString());
    bool tryLoadSkins();

    void open();
    void close();

    void hide();
    void show();

    void exit();

    void setCaption(QString caption);

    TLyricWindow *lyricWindow() { return mLyricWindow; }
    TMainWindow *mainWindow() { return mMainWindow; }
    TMainMenu *mainMenu() { return mMainMenu; }
    TEqualizerWindow *equalizerWindow() { return mEqualizerWindow; }
    TPlaylistWindow *playlistWindow() { return mPlaylistWindow; }
    TBrowserWindow *browserWindow() { return mBrowserWindow; }
    TDesktopLyricWindow *desktopLyricWindow() { return mDesktopLyricWindow; }
    TPropertyDialog *propertyDialog() { return mPropertyDialog; }
    TExportDialog *exportDialog() { return mExportDialog; }
    TExportMissionsDialog *exportMissionDialog() { return mExportMissionDialog; }
    TOptionsDialog *optionDialog() { return mOptionDialog; }

signals:
    void requestPlayNextMusic();
    void requestPlayPreviousMusic();
    void requestShutdown();
    void skinChanged();
    void muteToggled(bool);

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
    void slotOpenOptionsDialog();
    void slotAbout();
    void slotAboutQt();
    void slotVolumeToggled(bool);
    void slotOnOpacityChanged(qreal value);
    void slotRequestLoadSkin(int skinIndex);
    void slotRequestSkinNames(QStringList &names, int &currentIndex);
    void slotRequestSkinInfo(int,QPixmap&,QString&,QString&,QString&);

    void slotMainWindowActivationChanged();
    void slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void slotVolumeUpTriggered();
    void slotVolumeDownTriggered();
    void slotDisplayTrayIconToggled(bool);

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

    TPropertyDialog *mPropertyDialog;
    TExportDialog *mExportDialog;
    TExportMissionsDialog *mExportMissionDialog;
    TOptionsDialog *mOptionDialog;

    TMainMenu *mMainMenu;
    QSystemTrayIcon *mTrayIcon;

    TSkinManager *mSkinManager;

    bool mMinimode;
    bool mShowDesktopLyric;
    bool mBrowserVisible;
    bool mPlaylistVisible;
    bool mLyricWindowVisible;
    bool mDesktopLyricVisible;
    bool mEqualizerVisible;

    QString mCurrentSkinFile;

    void createSystemTrayIcon();
    bool isMainwindowSunken();
    void toggleGui();
    void restoreGui();

    // Move window to desktop center
    void centerWindow();
    void moveWindow(TAbstractWindow* window, int left, int top);
    bool loadSkin(TSkin *skin);
};

#endif // TGUIMANAGER_H
