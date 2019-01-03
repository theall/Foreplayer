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
#include "optionscontroller.h"

#include "preferences.h"

TOptionsController::TOptionsController(QObject *parent) :
    TAbstractController(parent)
  , mPluginModel(new TPluginModel(this))
  , mSuffixModel(new TSuffixModel(this))
{

}

TOptionsController::~TOptionsController()
{

}

bool TOptionsController::joint(TGuiManager *manager, TCore *core)
{
    Q_ASSERT(manager);
    Q_ASSERT(core);

    mPluginModel->setPlayerCore(core);

    manager->optionDialog()->optionPluginInfo()->setPluginModel(mPluginModel);

    connect(manager->optionDialog()->optionPluginInfo(),
            SIGNAL(currentPluginIndexChanged(int)),
            this,
            SLOT(slotCurrentPluginIndexChanged(int)));

    manager->optionDialog()->optionPluginInfo()->setSuffixModel(mSuffixModel);

    return TAbstractController::joint(manager, core);
}

void TOptionsController::slotCurrentPluginIndexChanged(int index)
{
    if(mCore)
    {
        PluginHandles plugins = mCore->getPluginHandles();
        if(index>=0 && index<plugins.size())
        {
            mSuffixModel->setSuffixDecription(mCore->getPluginSuffixDescription(plugins.at(index)));
        }
    }
}

void TOptionsController::slotTimerEvent()
{
}
