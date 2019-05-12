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
#ifndef TCHECKBOX_H
#define TCHECKBOX_H

#include <QCheckBox>
#include <QMouseEvent>
#include <QPaintEvent>

#include "../skin/skin.h"
#include "buttonimage.h"

class TCheckBox : public QCheckBox, TSkinReader
{
public:
    TCheckBox(QWidget *parent=0);
    ~TCheckBox();

    void setPixmapRect(QPixmap pixmap, QRect rect);
    void setPixmap(QPixmap pixmap);
    void setFontColor(QFont font, QColor color);

protected:
    void enterEvent(QEvent *);//鼠标进入事件
    void leaveEvent(QEvent *);//离开
    void mousePressEvent(QMouseEvent *e);//点击
    void mouseReleaseEvent(QMouseEvent *e);//释放

private:
    TButtonIcon *mButtonImages;

    void setIconAndSize(QIcon icon);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TCHECKBOX_H
