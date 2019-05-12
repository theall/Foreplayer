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
#ifndef TSLIDERBAR_H
#define TSLIDERBAR_H

#include <QSlider>
#include <QMouseEvent>

#include "style/sliderbarstyle.h"
#include "../skin/skin.h"

class TSliderBar : public QSlider, TSkinReader
{
    Q_OBJECT

    enum TPixmapType
    {
        PT_GROOVE,
        PT_BUTTON,
        PT_FILL,
        PixmapTypeCount
    };

public:
    explicit TSliderBar(QWidget *parent = 0);
    explicit TSliderBar(Qt::Orientation orientation, QWidget *parent = 0);
    ~TSliderBar();

    void setPixmap(TPixmapType type, QPixmap pixmap);
    void setPixmaps(QPixmap groove, QPixmap button, QPixmap fill);

    void setVertical(bool bVertical = false);

    const QPixmap *groovePixmap() const;
    const QPixmap *fillPixmap() const;
    const TButtonPixmap *buttonPixmap() const;

signals:

public slots:

private:
    QPixmap mGroovePixmap;
    QPixmap mFillPixmap;
    TButtonPixmap mButtonPixmap;
    TSliderbarStyle mStyle;
    void initialize();

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TSLIDERBAR_H
