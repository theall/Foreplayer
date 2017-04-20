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
#ifndef TIMAGEBUTTON_H
#define TIMAGEBUTTON_H

#include "buttonimage.h"
#include "../../share/skin.h"

class TImageButton : public QPushButton, TSkinReader
{
    Q_OBJECT

public:
    TImageButton(QWidget *parent=0);
    ~TImageButton();

    void setPixmapRect(QPixmap pixmap, QRect rect);
    void setPixmap(QPixmap pixmap);

    Qt::Alignment alignment();
    void setAlignment(QPixmap pixmap, Qt::Alignment alignment);

    void updatePos();

signals:
    void mouseEnter();
    void mouseLeave();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *eevent);
    void mouseReleaseEvent(QMouseEvent *eevent);

private:
    QSize mAlignSize;
    Qt::Alignment mAlignment;
    TButtonIcon *mButtonImages;

    void setIconAndSize(QIcon icon);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TIMAGEBUTTON_H
