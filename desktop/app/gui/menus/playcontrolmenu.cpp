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
#include "playcontrolmenu.h"

TPlayControlMenu::TPlayControlMenu(QWidget *parent) :
    TAbstractMenu(parent)
{
    mActionPlay = new QAction(this);
    mActionPause = new QAction(this);
    mActionStop = new QAction(this);
    mActionPrevTrack = new QAction(this);
    mActionNextTrack = new QAction(this);
    mActionPrevMusic = new QAction(this);
    mActionNextMusic = new QAction(this);
    mActionOpenFile = new QAction(this);

    connect(mActionPlay, SIGNAL(triggered()), this, SIGNAL(onPlayTriggered()));
    connect(mActionPause, SIGNAL(triggered()), this, SIGNAL(onPauseTriggered()));
    connect(mActionStop, SIGNAL(triggered()), this, SIGNAL(onStopTriggered()));
    connect(mActionPrevTrack, SIGNAL(triggered()), this, SIGNAL(onPrevTrackTriggered()));
    connect(mActionNextTrack, SIGNAL(triggered()), this, SIGNAL(onNextTrackTriggered()));
    connect(mActionPrevMusic, SIGNAL(triggered()), this, SIGNAL(onPrevMusicTriggered()));
    connect(mActionNextMusic, SIGNAL(triggered()), this, SIGNAL(onNextMusicTriggered()));
    connect(mActionOpenFile, SIGNAL(triggered()), this, SIGNAL(onOpenFileTriggered()));

    addAction(mActionPlay);
    addAction(mActionPause);
    addAction(mActionStop);
    addSeparator();
    addAction(mActionPrevTrack);
    addAction(mActionNextTrack);
    addAction(mActionPrevMusic);
    addAction(mActionNextMusic);
    addSeparator();
    addAction(mActionOpenFile);

    retranslateUi();
}

void TPlayControlMenu::retranslateUi()
{
    mActionPlay->setText(tr("Play"));
    mActionPause->setText(tr("Pause"));
    mActionStop->setText(tr("Stop"));
    mActionPrevTrack->setText(tr("Previous track/music"));
    mActionNextTrack->setText(tr("Next track/music"));
    mActionPrevMusic->setText(tr("Previous tracklist/music"));
    mActionNextMusic->setText(tr("Next tracklist/music"));
    mActionOpenFile->setText(tr("Open file"));
}
