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

TPlaymodeMenu::TPlaymodeMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mLastActivedAction(NULL)
{
    mActionSingleOnce = new QAction(this);
    mActionSingleRecycle = new QAction(this);
    mActionTracklistRecycle = new QAction(this);
    mActionTracklistShuffle = new QAction(this);
    mActionShuffleAll = new QAction(this);
    mActionRecycleAll = new QAction(this);
    mActionTracklistOnce = new QAction(this);
    mActionInorderOnce = new QAction(this);

    mActionSingleOnce->setCheckable(true);
    mActionSingleRecycle->setCheckable(true);
    mActionTracklistRecycle->setCheckable(true);
    mActionTracklistShuffle->setCheckable(true);
    mActionShuffleAll->setCheckable(true);
    mActionRecycleAll->setCheckable(true);
    mActionTracklistOnce->setCheckable(true);
    mActionInorderOnce->setCheckable(true);

    connect(mActionSingleOnce, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionSingleRecycle, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTracklistRecycle, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTracklistShuffle, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionShuffleAll, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionRecycleAll, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTracklistOnce, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionInorderOnce, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));

    addAction(mActionSingleOnce);
    addAction(mActionSingleRecycle);
    addSeparator();
    addAction(mActionTracklistOnce);
    addAction(mActionTracklistRecycle);
    addAction(mActionTracklistShuffle);
    addSeparator();
    addAction(mActionInorderOnce);
    addAction(mActionRecycleAll);
    addAction(mActionShuffleAll);

    retranslateUi();
}

void TPlaymodeMenu::slotActionTriggered(bool triggered)
{
    Q_UNUSED(triggered)

    QAction *action = static_cast<QAction*>(sender());

    if(mLastActivedAction)
        mLastActivedAction->setChecked(false);

    mLastActivedAction = action;

    if(action==mActionSingleOnce) {
        emit onSingleOnceTriggered();
    }
    else if(action==mActionSingleRecycle) {
        emit onSingleRecycleTriggered();
    }
    else if(action==mActionTracklistRecycle) {
        emit onTracklistRecycleTriggered();
    }
    else if(action==mActionTracklistShuffle) {
        emit onTracklistShuffleTriggered();
    }
    else if(action==mActionShuffleAll) {
        emit onShuffleAllTriggered();
    }
    else if(action==mActionRecycleAll) {
        emit onRecycleAllTriggered();
    }
    else if(action==mActionTracklistOnce) {
        emit onTracklistOnceTriggered();
    }
    else if(action==mActionInorderOnce) {
        emit onInorderOnceTriggered();
    }
}

void TPlaymodeMenu::retranslateUi()
{
    mActionSingleOnce->setText(tr("Track/Music once"));
    mActionSingleRecycle->setText(tr("Track/Music recycle"));
    mActionTracklistRecycle->setText(tr("Tracklist recycle"));
    mActionTracklistShuffle->setText(tr("Tracklist shuffle"));
    mActionShuffleAll->setText(tr("Shuffle all"));
    mActionRecycleAll->setText(tr("Recycle all"));
    mActionTracklistOnce->setText(tr("Tracklist once"));
    mActionInorderOnce->setText(tr("Inorder once"));
}
