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
#include "playmodemenu.h"

#include "preferences.h"

TPlaymodeMenu::TPlaymodeMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mLastActivedAction(NULL)
{
    mActionManual = new QAction(this);
    mActionRecycleTrack = new QAction(this);
    mActionRecycleTracklist = new QAction(this);
    mActionRecyclePlaylist = new QAction(this);
    mActionRecycleAll = new QAction(this);
    mActionRandom = new QAction(this);

    mActionManual->setCheckable(true);
    mActionRecycleTrack->setCheckable(true);
    mActionRecycleTracklist->setCheckable(true);
    mActionRecyclePlaylist->setCheckable(true);
    mActionRecycleAll->setCheckable(true);
    mActionRandom->setCheckable(true);

    connect(mActionManual, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionRecycleTrack, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionRecycleTracklist, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionRecyclePlaylist, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionRecycleAll, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionRandom, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));

    addAction(mActionManual);
    addSeparator();
    addAction(mActionRecycleTrack);
    addAction(mActionRecycleTracklist);
    addAction(mActionRecyclePlaylist);
    addSeparator();
    addAction(mActionRandom);

    retranslateUi();
}

TPlaymodeMenu::~TPlaymodeMenu()
{
    PlayMode pm = MANUAL;
    if(mActionRandom->isChecked())
        pm = RANDOM;
    else if(mActionRecycleAll->isChecked())
        pm = RECYCLE_ALL;
    else if(mActionRecycleTrack->isChecked())
        pm = RECYCLE_TRACK;
    else if(mActionRecycleTracklist->isChecked())
        pm = RECYCLE_TRACK_LIST;
    else if(mActionRecycleTracklist->isChecked())
        pm = RECYCLE_PLAY_LIST;
    else if(mActionManual->isChecked())
        pm = MANUAL;

    TPreferences::instance()->setPlayMode(pm);
}

void TPlaymodeMenu::loadSettings()
{
    PlayMode pm = TPreferences::instance()->playMode();
    if(pm==RANDOM)
        mActionRandom->trigger();
    else if(pm==RECYCLE_ALL)
        mActionRecycleAll->trigger();
    else if(pm==RECYCLE_TRACK_LIST)
        mActionRecycleTracklist->trigger();
    else if(pm==RECYCLE_TRACK)
        mActionRecycleTrack->trigger();
    else if(pm==RECYCLE_PLAY_LIST)
        mActionRecyclePlaylist->trigger();
    else
        mActionManual->trigger();
}

void TPlaymodeMenu::slotActionTriggered(bool triggered)
{
    Q_UNUSED(triggered)

    QAction *action = static_cast<QAction*>(sender());

    if(mLastActivedAction)
        mLastActivedAction->setChecked(false);

    mLastActivedAction = action;

    PlayMode pm = MANUAL;
    if(action==mActionRecycleTrack) {
        pm = RECYCLE_TRACK;
    } else if(action==mActionRecycleTracklist) {
        pm = RECYCLE_TRACK_LIST;
    } else if(action==mActionRecyclePlaylist) {
        pm = RECYCLE_PLAY_LIST;
    } else if(action==mActionRecycleAll) {
        pm = RECYCLE_ALL;
    } else if(action==mActionRandom) {
        pm = RANDOM;
    } else if(action==mActionManual) {
        pm = MANUAL;
    }

    TPreferences::instance()->setPlayMode(pm);
}

void TPlaymodeMenu::retranslateUi()
{
    mActionManual->setText(tr("Manual"));
    mActionRecycleTrack->setText(tr("Recycle track"));
    mActionRecycleTracklist->setText(tr("Recycle tracklist"));
    mActionRecyclePlaylist->setText(tr("Recycle playlist"));
    mActionRecycleAll->setText(tr("Recycle all"));
    mActionRandom->setText(tr("Random"));
}
