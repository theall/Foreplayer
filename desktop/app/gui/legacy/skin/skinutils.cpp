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

#include "skinutils.h"

namespace SkinUtils {

    bool strToBool(QString s)
    {
        bool result = false;
        s = s.trimmed();
        if(s=="1" || s.toLower()=="true")
            result = true;

        return result;
    }

    Qt::Alignment strToAlign(QString s)
    {
        Qt::Alignment i;
        s = s.toLower();
        if(s.indexOf("center")>=0)
            i |= Qt::AlignHCenter;
        if(s.indexOf("left")>=0)
            i |= Qt::AlignLeft;
        if(s.indexOf("top")>=0)
            i |= Qt::AlignTop;
        if(s.indexOf("right")>=0)
            i |= Qt::AlignRight;
        if(s.indexOf("bottom")>=0)
            i |= Qt::AlignBottom;
        return i;
    }

    QRect strToGeometry(QString s)
    {
        QRect rect;
        QStringList sl = s.split(",");
        if(sl.size()!=4)
        {
            return rect;
        }
        rect.setLeft(sl[0].toInt());
        rect.setTop(sl[1].toInt());
        rect.setRight(sl[2].toInt());
        rect.setBottom(sl[3].toInt());
        return rect;
    }

    QRect strToRect(QString s)
    {
        QRect rect;
        QStringList sl = s.split(",");
        if(sl.size()==4)
        {
            rect.setLeft(0);
            rect.setTop(0);
            rect.setWidth(sl[2].toInt()-sl[0].toInt());
            rect.setHeight(sl[3].toInt()-sl[1].toInt());
        }

        return rect;
    }

    QFont strToFont(QString s)
    {
        QFont font;
        QStringList sl = s.split(",");
        if(sl.size() == 14)
        {

        }
        return font;
    }

    QRect extractGeometry(QDomElement e, QString attr)
    {
        return strToGeometry(e.attribute(attr));
    }

    QRect extractResizeRect(QDomElement e, QString attr)
    {
        return strToGeometry(e.attribute(attr));
    }

    Qt::Alignment extractAlignment(QDomElement e, QString attr)
    {
        return strToAlign(e.attribute(attr));
    }

    QFont extractFont(QDomElement e)
    {
        QFont ft;
        QString family = e.attribute(ATTR_FONT);
        if(!family.isEmpty())
            ft.setFamily(family);
        int fontSize = e.attribute(ATTR_FONT_SIZE).toInt();
        if(fontSize > 0)
            ft.setPixelSize(fontSize);
        return ft;
    }

} // namespace SkinUtils
