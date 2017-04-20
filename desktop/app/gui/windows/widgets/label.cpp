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
#include "label.h"

TLabel::TLabel(QWidget *parent):
    QLabel(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setScaledContents(true);
    setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
}

void TLabel::setFontColor(QFont font, QColor color)
{
    setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText, color);
    setPalette(pa);
}

void TLabel::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    setGeometry(SkinUtils::extractGeometry(element));

    QPixmap p = skin->findPixmap(element.attribute(ATTR_ICON));
    if(!p.isNull())
        setPixmap(p);

    setFontColor(SkinUtils::extractFont(element), QColor(element.attribute(ATTR_COLOR)));

    setAlignment(SkinUtils::extractAlignment(element));
}

void TLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        emit clicked();

    QLabel::mousePressEvent(event);
}
