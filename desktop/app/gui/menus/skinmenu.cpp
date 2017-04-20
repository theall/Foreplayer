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
#include "skinmenu.h"
#include "../share/skin.h"

#define BUF_SIZE 256

TSkinMenu::TSkinMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mLastActivedAction(NULL)
{

}

void TSkinMenu::slotSkinTriggered(bool checked)
{
    QAction *action = qobject_cast<QAction*>(sender());

    if(checked)
    {
        if(mLastActivedAction)
        {
            mLastActivedAction->blockSignals(true);
            mLastActivedAction->setChecked(false);
            mLastActivedAction->blockSignals(false);
        }
        mLastActivedAction = action;

        if(action)
            emit requestLoadSkin(action->data().toInt());
    }
}

void TSkinMenu::retranslateUi()
{
}

void TSkinMenu::showEvent(QShowEvent *event)
{
    clear();
    QStringList skinNames;
    emit requestSkinNames(skinNames);
    for(int i=0;i<skinNames.count();i++)
    {
        QAction *action = addAction(skinNames[i], this, SLOT(slotSkinTriggered(bool)));
        action->setCheckable(true);
        action->setData(i);
    }
    TAbstractMenu::showEvent(event);
}
