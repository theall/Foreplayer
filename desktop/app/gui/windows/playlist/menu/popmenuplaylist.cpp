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
#include "popmenuplaylist.h"

TPopMenuPlayList::TPopMenuPlayList(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionAdd = addAction(QString(), this, SIGNAL(onActionAddTriggered()));
    mActionRemove = addAction(QString(), this, SIGNAL(onActionRemoveTriggered()));
    mActionRename = addAction(QString(), this, SIGNAL(onActionRenameTriggered()));
    mActionSort = addAction(QString(), this, SIGNAL(onActionSortTriggered()));
    mActionSend = addAction(QString(), this, SIGNAL(onActionSendTriggered()));

    mActionSend->setVisible(false);
    retranslateUi();
}

TPopMenuPlayList::~TPopMenuPlayList()
{

}

void TPopMenuPlayList::display(const QPoint &pos, int selections)
{
    bool hasSelection = selections>0;
    mActionAdd->setEnabled(true);
    mActionRemove->setEnabled(hasSelection);
    mActionRename->setEnabled(selections==1);
    mActionSort->setEnabled(!hasSelection);

    popup(pos);
}

void TPopMenuPlayList::showEvent(QShowEvent *ev)
{
    QMenu::showEvent(ev);
}

void TPopMenuPlayList::retranslateUi()
{
    setTitle(tr("Playlist"));

    mActionAdd->setText(tr("Add"));
    mActionRemove->setText(tr("Remove"));
    mActionRename->setText(tr("Rename"));
    mActionSort->setText(tr("Sort"));
    mActionSend->setText(tr("Send"));
}
