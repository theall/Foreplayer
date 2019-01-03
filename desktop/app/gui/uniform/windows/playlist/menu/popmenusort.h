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
#ifndef TPOPMENUSORT_H
#define TPOPMENUSORT_H

#include "abstractpopmenu.h"

class TPopMenuSort : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuSort(QWidget *parent = 0);
    ~TPopMenuSort();

signals:
    void onActionSortAsNameTriggered();
    void onActionSortAsArtistTriggered();
    void onActionSortAsAlbumTriggered();
    void onActionSortAsDurationTriggered();
    void onActionSortRandomTriggered();
    void onActionSortReverseTriggered();

private:
    QAction *mActionSortAsName;
    QAction *mActionSortAsArtist;
    QAction *mActionSortAsAlbum;
    QAction *mActionSortAsDuration;
    QAction *mActionSortRandom;
    QAction *mActionSortReverse;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUSORT_H
