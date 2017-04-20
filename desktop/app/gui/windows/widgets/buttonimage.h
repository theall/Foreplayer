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
#ifndef TBUTTONIMAGE_H
#define TBUTTONIMAGE_H

#include "pch.h"

enum TButtonIndex
{
    BI_Normal,
    BI_Hover,
    BI_MouseDown,
    BI_Disabled,
    TBUTTON_COUNT
};

class TButtonIcon
{
public:
    explicit TButtonIcon();

    void setPixmap(QPixmap pixmap, int count=TBUTTON_COUNT);
    void seIcon(TButtonIndex index, QIcon icon);

    const QIcon *normal() const;
    const QIcon *hover() const;
    const QIcon *mouseDown() const;
    const QIcon *diabled() const;

    QSize size();

private:
    QSize mSize;
    QVector<QIcon> mIcons;
};

class TButtonPixmap
{
public:
    explicit TButtonPixmap();

    void setPixmap(QPixmap pixmap, int count=TBUTTON_COUNT);
    void setPixmap(TButtonIndex index, QPixmap pixmap);

    const QPixmap *normal() const;
    const QPixmap *hover() const;
    const QPixmap *mouseDown() const;
    const QPixmap *diabled() const;

    const QSize size() const;

private:
    QSize mSize;
    QVector<QPixmap> mPixmaps;
};
#endif // TBUTTONIMAGE_H
