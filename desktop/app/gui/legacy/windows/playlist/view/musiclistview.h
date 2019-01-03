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
#ifndef TMUSICLISTVIEW_H
#define TMUSICLISTVIEW_H

#include "abstracttableview.h"

class TMusiclistView : public TAbstractTableView
{
public:
    TMusiclistView(QWidget *parent = 0);

    void editCurrentName();
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;
    void updateColumnsWidth() Q_DECL_OVERRIDE;
};

#endif // TMUSICLISTVIEW_H
