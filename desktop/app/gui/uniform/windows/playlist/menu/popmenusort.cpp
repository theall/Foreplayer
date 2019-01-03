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
#include "popmenusort.h"

TPopMenuSort::TPopMenuSort(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionSortAsName = addAction(QString(), this, SIGNAL(onActionSortAsNameTriggered()));
    mActionSortAsArtist = addAction(QString(), this, SIGNAL(onActionSortAsArtistTriggered()));
    mActionSortAsAlbum = addAction(QString(), this, SIGNAL(onActionSortAsAlbumTriggered()));
    mActionSortAsDuration = addAction(QString(), this, SIGNAL(onActionSortAsDurationTriggered()));
    addSeparator();
    mActionSortRandom = addAction(QString(), this, SIGNAL(onActionSortRandomTriggered()));
    mActionSortReverse = addAction(QString(), this, SIGNAL(onActionSortReverseTriggered()));
    retranslateUi();
}

TPopMenuSort::~TPopMenuSort()
{

}

void TPopMenuSort::retranslateUi()
{
    setTitle(tr("Sort"));

    mActionSortAsName->setText(tr("As title"));
    mActionSortAsArtist->setText(tr("As artist"));
    mActionSortAsAlbum->setText(tr("As album"));
    mActionSortAsDuration->setText(tr("As length"));
    mActionSortRandom->setText(tr("Random"));
    mActionSortReverse->setText(tr("Reverse"));
}
