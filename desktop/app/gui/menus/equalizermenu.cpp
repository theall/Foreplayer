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
#include "equalizermenu.h"

TEqualizerMenu::TEqualizerMenu(QWidget *parent) :
    TAbstractMenu(parent)
  , mLastActivedAction(NULL)
{
    mActionRock = new QAction(this);
    mActionMetal = new QAction(this);
    mActionElectric = new QAction(this);
    mActionPop = new QAction(this);
    mActionJazz = new QAction(this);
    mActionClassic = new QAction(this);
    mActionCustomize = new QAction(this);

    mActionRock->setCheckable(true);
    mActionMetal->setCheckable(true);
    mActionElectric->setCheckable(true);
    mActionPop->setCheckable(true);
    mActionJazz->setCheckable(true);
    mActionClassic->setCheckable(true);
    mActionCustomize->setCheckable(true);

    addAction(mActionRock);
    addAction(mActionMetal);
    addAction(mActionElectric);
    addAction(mActionPop);
    addAction(mActionJazz);
    addAction(mActionClassic);
    addSeparator();
    addAction(mActionCustomize);
}

void TEqualizerMenu::slotActionTriggered(bool)
{
    QAction *action = static_cast<QAction*>(sender());

    if(mLastActivedAction)
        mLastActivedAction->setChecked(false);

    mLastActivedAction = action;

    if(action==mActionRock) {
       emit onRockSelected();
    }
    else if(action==mActionMetal) {
       emit onMetalSelected();
    }
    else if(action==mActionElectric) {
       emit onElectricSelected();
    }
    else if(action==mActionPop) {
       emit onPopSelected();
    }
    else if(action==mActionJazz) {
       emit onJazzSelected();
    }
    else if(action==mActionClassic) {
       emit onClassicSelected();
    }
    else if(action==mActionCustomize) {
       emit onCustomizeSelected();
    }
}

void TEqualizerMenu::retranslateUi()
{
    mActionRock->setText(tr("Rock"));
    mActionMetal->setText(tr("Metal"));
    mActionElectric->setText(tr("Electric"));
    mActionPop->setText(tr("Pop"));
    mActionJazz->setText(tr("Jazz"));
    mActionClassic->setText(tr("Classic"));
    mActionCustomize->setText(tr("Customize"));
}
