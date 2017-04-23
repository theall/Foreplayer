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
#include "maincontroller.h"

#include "preferences.h"

#include <QMessageBox>

TMainController::TMainController(QObject *parent) :
    TAbstractController(parent)
  , mPlayerController(new TPlayerController(this))
  , mPlaylistController(new TPlaylistController(this))
  , mEqualizerController(new TEqualizerController(this))

{
    connect(mPlaylistController,
            SIGNAL(requestPlay(int,int,int)),
            mPlayerController,
            SLOT(slotRequestPlay(int,int,int)));

    connect(mPlayerController,
            SIGNAL(requestUpdateModelsPlayingIndex(int,int,int)),
            mPlaylistController,
            SLOT(slotRequestUpdateModelsPlayingIndex(int,int,int)));

    connect(mPlayerController,
            SIGNAL(requestCurrentIndex(int*,int*,int*)),
            mPlaylistController,
            SLOT(slotRequestCurrentIndex(int*,int*,int*)));

    connect(mPlayerController,
            SIGNAL(requestFixDuration(int)),
            mPlaylistController,
            SLOT(slotRequestFixDuration(int)));
}

TMainController::~TMainController()
{

}

bool TMainController::joint(TGuiManager *manager, TCore *core)
{
    Q_ASSERT(manager);
    Q_ASSERT(core);

    connect(manager, SIGNAL(requestShutdown()), this, SLOT(slotQuitApp()));

//    if(!manager->tryLoadSkins())
//        return false;

    mPlayerController->joint(manager, core);
    mPlaylistController->joint(manager, core);
    mEqualizerController->joint(manager, core);

    manager->open();

    return TAbstractController::joint(manager, core);
}

void TMainController::slotQuitApp()
{
    qApp->quit();
}

void TMainController::slotTimerEvent()
{
}
