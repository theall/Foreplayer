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
#include "buttonimage.h"

/**
 * @brief TButtonIcon::TButtonIcon
 * @param parent
 */
TButtonIcon::TButtonIcon()
{
    for(int i=0;i<TBUTTON_COUNT;i++)
    {
        mIcons.append(QIcon());
    }
}

void TButtonIcon::setPixmap(QPixmap pixmap, int count)
{
    if(count<1)
        return;

    int x, y;
    int width, uwidth;
    int height;
    width = pixmap.width();
    height = pixmap.height();
    uwidth = width / TBUTTON_COUNT;

    x = 0;
    y = 0;

    mIcons.clear();
    for(int i=0;i<TBUTTON_COUNT;i++)
    {
        QPixmap p = pixmap.copy(x, y, uwidth, height);
        mIcons.append(QIcon(p));
        x += uwidth;
    }

    for(int i=0;i<TBUTTON_COUNT-count;i++)
    {
        mIcons.append(mIcons[0]);
    }

    mSize.setWidth(uwidth);
    mSize.setHeight(height);
}

void TButtonIcon::seIcon(TButtonIndex index, QIcon icon)
{
    int i = (int)index;
    if(i<0 || i>=mIcons.size())
        return;

    mIcons[index] = icon;
}

const QIcon *TButtonIcon::normal() const
{
    return &mIcons[BI_Normal];
}

const QIcon *TButtonIcon::hover() const
{
    return &mIcons[BI_Hover];
}

const QIcon *TButtonIcon::mouseDown() const
{
    return &mIcons[BI_MouseDown];
}

const QIcon *TButtonIcon::diabled() const
{
    return &mIcons[BI_Disabled];
}

QSize TButtonIcon::size()
{
    return mSize;
}


/**
 * @brief TButtonPixmap::TButtonPixmap
 * @param parent
 */
TButtonPixmap::TButtonPixmap()
{
    for(int i=0;i<TBUTTON_COUNT;i++)
    {
        mPixmaps.append(QPixmap());
    }
}

void TButtonPixmap::setPixmap(QPixmap pixmap, int count)
{
    if(count<1)
        return;

    int x, y;
    int width, uwidth;
    int height;
    width = pixmap.width();
    height = pixmap.height();
    uwidth = width / count;

    x = 0;
    y = 0;

    mPixmaps.clear();
    for(int i=0;i<count;i++)
    {
        mPixmaps.append(pixmap.copy(x, y, uwidth, height));
        x += uwidth;
    }

    for(int i=0;i<TBUTTON_COUNT-count;i++)
    {
        mPixmaps.append(mPixmaps[0]);
    }
    mSize.setWidth(uwidth);
    mSize.setHeight(height);
}

void TButtonPixmap::setPixmap(TButtonIndex index, QPixmap pixmap)
{
    int i = (int)index;
    if(i<0 || i>=mPixmaps.size())
        return;

    mPixmaps[index] = pixmap;
}

const QPixmap *TButtonPixmap::normal() const
{
    return &mPixmaps[BI_Normal];
}

const QPixmap *TButtonPixmap::hover() const
{
    return &mPixmaps[BI_Hover];
}

const QPixmap *TButtonPixmap::mouseDown() const
{
    return &mPixmaps[BI_MouseDown];
}

const QPixmap *TButtonPixmap::diabled() const
{
    return &mPixmaps[BI_Disabled];
}

const QSize TButtonPixmap::size() const
{
    return mSize;
}
