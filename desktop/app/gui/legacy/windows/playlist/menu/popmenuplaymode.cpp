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
#include "popmenuplaymode.h"

TPopMenuPlayMode::TPopMenuPlayMode(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionSingleOnce = addAction(QString(), this, SIGNAL(onActionSingleOnceTriggered()));
    mActionSignleRecycle = addAction(QString(), this, SIGNAL(onActionSignleRecycleTriggered()));
    mActionAllOnce = addAction(QString(), this, SIGNAL(onActionAllOnceTriggered()));
    mActionAllRecycle = addAction(QString(), this, SIGNAL(onActionAllRecycleTriggered()));
    mActionRandom = addAction(QString(), this, SIGNAL(onActionRandomTriggered()));
    mActionAutoSwitch = addAction(QString(), this, SIGNAL(onActionAutoSwitchTriggered()));
    retranslateUi();
}

TPopMenuPlayMode::~TPopMenuPlayMode()
{

}

void TPopMenuPlayMode::retranslateUi()
{
    setTitle(tr("Play Mode"));
    mActionSingleOnce->setText(tr("Single once"));
    mActionSignleRecycle->setText(tr("Signle recycle"));
    mActionAllOnce->setText(tr("All once"));
    mActionAllRecycle->setText(tr("All recycle"));
    mActionRandom->setText(tr("Random"));
    mActionAutoSwitch->setText(tr("Auto switch"));
}
