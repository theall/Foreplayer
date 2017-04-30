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

#include "preferences.h"

enum ProfileIndex
{
    PI_CUSSTOMIZE,
    PI_ROCK,
    PI_METAL,
    PI_ELECTRIC,
    PI_POP,
    PI_JAZZ,
    PI_CLASSIC
};

void checkAction(QAction *action, bool checked)
{
    if(action)
    {
        action->blockSignals(true);
        action->setChecked(checked);
        action->blockSignals(false);
    }
}


TEqualizerMenu::TEqualizerMenu(QWidget *parent) :
    TAbstractMenu(parent, true)
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

    connect(mActionRock, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionMetal, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionElectric, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionPop, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionJazz, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionClassic, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionCustomize, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));

    addAction(mActionRock);
    addAction(mActionMetal);
    addAction(mActionElectric);
    addAction(mActionPop);
    addAction(mActionJazz);
    addAction(mActionClassic);
    addSeparator();
    addAction(mActionCustomize);
}

TEqualizerMenu::~TEqualizerMenu()
{
    ProfileIndex profileIndex;
    if (mActionRock->isChecked())
        profileIndex = PI_ROCK;
    else if (mActionMetal->isChecked())
        profileIndex = PI_METAL;
    else if (mActionElectric->isChecked())
        profileIndex = PI_ELECTRIC;
    else if (mActionPop->isChecked())
        profileIndex = PI_POP;
    else if (mActionJazz->isChecked())
        profileIndex = PI_JAZZ;
    else if (mActionClassic->isChecked())
        profileIndex = PI_CLASSIC;
    else
        profileIndex = PI_CUSSTOMIZE;

    TPreferences::instance()->setEqProfile((int)profileIndex);
}

void TEqualizerMenu::loadSettings()
{
    ProfileIndex profileIndex = (ProfileIndex)TPreferences::instance()->eqProfile();
    if (profileIndex==PI_ROCK)
        mActionRock->trigger();
    else if (profileIndex==PI_METAL)
        mActionMetal->trigger();
    else if (profileIndex==PI_ELECTRIC)
        mActionElectric->trigger();
    else if (profileIndex==PI_POP)
        mActionPop->trigger();
    else if (profileIndex==PI_JAZZ)
        mActionJazz->trigger();
    else if (profileIndex==PI_CLASSIC)
        mActionClassic->trigger();
    else
        mActionCustomize->trigger();
}

bool TEqualizerMenu::isCustomizeActionChecked()
{
    return mActionCustomize->isChecked();
}

void TEqualizerMenu::checkCustomizeAction()
{
    if(mLastActivedAction)
        checkAction(mLastActivedAction, false);

    checkAction(mActionCustomize, true);

    mLastActivedAction = mActionCustomize;
}

void TEqualizerMenu::slotActionTriggered(bool checked)
{
    QAction *action = static_cast<QAction*>(sender());

    if(!checked)
    {
        if(action == mActionCustomize)
        {
            checkAction(mActionCustomize, true);
        } else {
            mActionCustomize->trigger();
        }
        return;
    }

    if(mLastActivedAction)
        checkAction(mLastActivedAction, false);


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
