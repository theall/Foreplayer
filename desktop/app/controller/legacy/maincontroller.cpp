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
#include <QApplication>

TMainController::TMainController(QObject *parent) :
    TAbstractController(parent)
  , mPlayerController(new TPlayerController(this))
  , mPlaylistController(new TPlaylistController(this))
  , mEqualizerController(new TEqualizerController(this))
  , mOptionsController(new TOptionsController(this))
  , mExportController(new TExportController(this))
{
    connect(mPlaylistController,
            SIGNAL(requestPlay(int,int,int)),
            mPlayerController,
            SLOT(slotRequestPlay(int,int,int)));

    connect(mPlaylistController,
            SIGNAL(requestExport(void*,int)),
            mExportController,
            SLOT(slotRequestAddExportMissions(void*,int)));

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

    connect(manager, SIGNAL(requestShutdown(bool&)), this, SLOT(slotQuitApp(bool&)));

    mPlayerController->joint(manager, core);
    mPlaylistController->joint(manager, core);
    mEqualizerController->joint(manager, core);
    mOptionsController->joint(manager, core);
    mExportController->joint(manager, core);

    return TAbstractController::joint(manager, core);
}

void TMainController::slotQuitApp(bool& approved)
{
    if(mExportController && mExportController->hasExportingMissions())
    {
        if(QMessageBox::question(
                        mGui->mainWindow(),
                        tr("Question"),
                        tr("There are sound tracks in exporting, do you really want to exit?")) != QMessageBox::Yes)
        {
            approved = false;
            return;
        }
    }
    approved = true;
    qApp->quit();
}

void TMainController::slotTimerEvent()
{
}
