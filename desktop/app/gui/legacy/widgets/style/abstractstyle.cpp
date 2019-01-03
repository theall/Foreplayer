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
#include "abstractstyle.h"

TAbstractStyle::TAbstractStyle():
    QCommonStyle()
{

}

void TAbstractStyle::polish(QWidget *widget){
    widget->setAttribute(Qt::WA_Hover, true);
}
void TAbstractStyle::unpolish(QWidget *widget){
    widget->setAttribute(Qt::WA_Hover, false);
}

void TAbstractStyle::drawButton(QPainter *painter,
                              const TButtonPixmap *buttonPixmap,
                              QRect buttonRect,
                              const QStyleOptionComplex *optionStyle,
                              bool isActived,
                              bool autoFill) const
{
    if(!buttonPixmap)
        return;

    const QPixmap *normal = buttonPixmap->normal();
    const QPixmap *hover = buttonPixmap->hover();
    const QPixmap *mouseDown = buttonPixmap->mouseDown();
    const QPixmap *disabled = buttonPixmap->diabled();
    const QPixmap *drawPixmap = normal;

    if(isActived)
    {
        if(optionStyle->state & QStyle::State_MouseOver)
        {
            if(optionStyle->state & State_Sunken)
                drawPixmap = mouseDown;
            else
                drawPixmap = hover;
        }
    } else {
        drawPixmap = disabled;
    }

    if(!drawPixmap->isNull())
    {
        if(!autoFill) {
            int x = buttonRect.x() + (float)(buttonRect.width() - drawPixmap->width()) / 2;
            int y = buttonRect.y() + (float)(buttonRect.height() - drawPixmap->height()) / 2;
            painter->drawPixmap(x, y, *drawPixmap);
        }
        else
        {
            int pixmapHeight = drawPixmap->height();
            int pixmapWidth = drawPixmap->width();
            int x = (float)(buttonRect.width() - pixmapWidth) / 2;
            int y = buttonRect.y();
            if(buttonRect.height() < pixmapHeight)
            {
                painter->drawPixmap(x, y, pixmapWidth, buttonRect.height(), *drawPixmap);
            } else {
                int headHeight = (float)pixmapHeight / 3;
                if(pixmapHeight%3>0)
                    headHeight++;

                int fillPixmapHeight = pixmapHeight - headHeight*2;
                QPixmap headPixmap = drawPixmap->copy(0, 0, pixmapWidth, headHeight);
                QPixmap fillPixmap = drawPixmap->copy(0, headHeight, pixmapWidth, fillPixmapHeight);

                int bottom = buttonRect.bottom();
                painter->drawPixmap(x, y, headPixmap);
                y += headHeight;

                int fillCount = (float)(buttonRect.height() - headHeight*2) / fillPixmapHeight + 0.5;
                for(int i=0;i<fillCount;i++)
                {
                    painter->drawPixmap(x, y, fillPixmap);
                    y += fillPixmapHeight;
                }
                headHeight = bottom - y;
                painter->drawPixmap(x, bottom-headHeight, drawPixmap->copy(0, pixmapHeight-headHeight, pixmapWidth, headHeight));
            }
        }
    }
}
