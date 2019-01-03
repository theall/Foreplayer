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
#include "musiclistview.h"

#define COLUMN_ICON             0
#define COLUMN_INDEX            1
#define COLUMN_TITLE            2
#define COLUMN_DURATION         3
#define COLUMN_ICON_WIDTH       6

TMusiclistView::TMusiclistView(QWidget *parent) :
    TAbstractTableView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TMusiclistView::editCurrentName()
{
    edit(currentIndex());
}

void TMusiclistView::setModel(QAbstractItemModel *model)
{
    TAbstractTableView::setModel(model);

    updateColumnsWidth();
}

void TMusiclistView::updateColumnsWidth()
{
    setColumnWidth(COLUMN_ICON, COLUMN_ICON_WIDTH);
    resizeColumnToContents(COLUMN_INDEX);
    resizeColumnToContents(COLUMN_DURATION);

    int w = rect().width()-columnWidth(COLUMN_INDEX)-columnWidth(COLUMN_DURATION)-COLUMN_ICON_WIDTH;
    QScrollBar *bar = verticalScrollBar();
    if(bar->isVisible())
        w -= bar->width()+3;

    setColumnWidth(COLUMN_TITLE, w);
}
