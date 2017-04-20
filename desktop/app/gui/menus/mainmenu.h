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
#ifndef TMAINMENU_H
#define TMAINMENU_H

#include "pch.h"
#include "abstractmenu.h"
#include "playcontrolmenu.h"
#include "volumecontrolmenu.h"
#include "playmodemenu.h"
#include "skinmenu.h"
#include "transparentmenu.h"

class TMainMenu : public TAbstractMenu
{
    Q_OBJECT

public:
    TMainMenu(QWidget *parent = 0);
    ~TMainMenu();

    TPlayControlMenu *playControlMenu();
    TVolumeControlMenu *volumeControlMenu();
    TPlaymodeMenu *playmodeMenu();
    TSkinMenu *skinMenu();
    TTransparentMenu *transparentMenu();

signals:
    void onVolumeUpTriggered();
    void onVolumeDownTriggered();
    void onVolumeMuteTriggered();
    void onViewLyricTriggered();
    void onViewPlaylistTriggered();
    void onViewEqualizerTriggered();
    void onViewMinimodeTriggered();
    void onRearrangeWindowsTriggered();
    void onAlwaysOnTopTriggered();
    void onFullLyricTriggered();
    void onFullSpectrumTriggered();
    void onFullBothTriggered();
    void onOptionsTriggered();
    void onLinksWebsiteTriggered();
    void onLinksHelpTriggered();
    void onAboutTriggered();
    void onAboutQtTriggered();
    void onExitTriggered();

private:
    TPlayControlMenu *mPlayingMenu;
    TVolumeControlMenu *mVolumeMenu;
    TPlaymodeMenu *mPlaymodeMenu;
    TSkinMenu *mSkinMenu;
    TTransparentMenu *mTransparentMenu;

    QMenu *mViewMenu;
    QMenu *mFullScreenMenu;
    QMenu *mAboutMenu;

    QAction *mActionAbout;
    QAction *mActionAboutQt;
    QAction *mActionOptions;
    QAction *mActionExit;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TMAINMENU_H
