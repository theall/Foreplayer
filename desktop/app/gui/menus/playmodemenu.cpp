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
    addAction(mActionRecycleAll);
    addSeparator();
    addAction(mActionRandom);

    retranslateUi();
}

TPlaymodeMenu::~TPlaymodeMenu()
{
    PlayMode pm = PM_MANUAL;
    if(mActionRandom->isChecked())
        pm = PM_RANDOM;
    else if(mActionRecycleAll->isChecked())
        pm = PM_RECYCLE_ALL;
    else if(mActionRecycleTrack->isChecked())
        pm = PM_RECYCLE_TRACK;
    else if(mActionRecycleTracklist->isChecked())
        pm = PM_RECYCLE_TRACK_LIST;
    else if(mActionRecycleTracklist->isChecked())
        pm = PM_RECYCLE_PLAY_LIST;
    else if(mActionManual->isChecked())
        pm = PM_MANUAL;

    TPreferences::instance()->setPlayMode(pm);
}

void TPlaymodeMenu::loadSettings()
{
    PlayMode pm = TPreferences::instance()->playMode();
    if(pm==PM_RANDOM)
        mActionRandom->trigger();
    else if(pm==PM_RECYCLE_ALL)
        mActionRecycleAll->trigger();
    else if(pm==PM_RECYCLE_TRACK_LIST)
        mActionRecycleTracklist->trigger();
    else if(pm==PM_RECYCLE_TRACK)
        mActionRecycleTrack->trigger();
    else if(pm==PM_RECYCLE_PLAY_LIST)
        mActionRecyclePlaylist->trigger();
    else
        mActionManual->trigger();
}

void TPlaymodeMenu::slotActionTriggered(bool checked)
{
    QAction *action = static_cast<QAction*>(sender());

    if(!action)
        return;

    if(!checked)
    {
        mActionManual->trigger();
        return;
    }

    if(mLastActivedAction)
    {
        mLastActivedAction->blockSignals(true);
        mLastActivedAction->setChecked(false);
        mLastActivedAction->blockSignals(false);
    }

    mLastActivedAction = action;

    PlayMode pm = PM_MANUAL;
    if(action==mActionRecycleTrack) {
        pm = PM_RECYCLE_TRACK;
    } else if(action==mActionRecycleTracklist) {
        pm = PM_RECYCLE_TRACK_LIST;
    } else if(action==mActionRecyclePlaylist) {
        pm = PM_RECYCLE_PLAY_LIST;
    } else if(action==mActionRecycleAll) {
        pm = PM_RECYCLE_ALL;
    } else if(action==mActionRandom) {
        pm = PM_RANDOM;
    } else if(action==mActionManual) {
        pm = PM_MANUAL;
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
