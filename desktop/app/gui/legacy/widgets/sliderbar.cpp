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
#include "sliderbar.h"

TSliderBar::TSliderBar(QWidget *parent) :
    QSlider(parent)
{
    initialize();
}

TSliderBar::TSliderBar(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent)
{
    initialize();
}

void TSliderBar::initialize()
{
    setCursor(Qt::PointingHandCursor);
    setRange(0, 100);
    setSingleStep(1);
    setValue(100);
    setMouseTracking(true);
    setStyle(&mStyle);
}

TSliderBar::~TSliderBar()
{
}

void TSliderBar::setPixmap(TSliderBar::TPixmapType type, QPixmap pixmap)
{
    if(type==PT_GROOVE)
        mGroovePixmap = pixmap;
    else if (type==PT_BUTTON)
        mButtonPixmap.setPixmap(pixmap);
    else if (type==PT_FILL)
        mFillPixmap = pixmap;
}

void TSliderBar::setPixmaps(QPixmap groove, QPixmap button, QPixmap fill)
{
    mGroovePixmap = groove;
    mFillPixmap = fill;
    mButtonPixmap.setPixmap(button);
}

void TSliderBar::setVertical(bool bVertical)
{
    setOrientation(bVertical ? Qt::Vertical : Qt::Horizontal);
}

const QPixmap *TSliderBar::groovePixmap() const
{
    return &mGroovePixmap;
}

const QPixmap *TSliderBar::fillPixmap() const
{
    return &mFillPixmap;
}

const TButtonPixmap *TSliderBar::buttonPixmap() const
{
    return &mButtonPixmap;
}

void TSliderBar::mouseMoveEvent(QMouseEvent *e)
{
    QSlider::mouseMoveEvent(e);

    // The slider bar will not send active sub control message, so force update.
    update();
}

void TSliderBar::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    QRect rt = SkinUtils::extractGeometry(element);
    setGeometry(rt);
    mGroovePixmap = skin->findPixmap(element.attribute(ATTR_BAR_IMAGE));
    mFillPixmap = skin->findPixmap(element.attribute(ATTR_FILL_IMAGE));
    mButtonPixmap.setPixmap(skin->findPixmap(element.attribute(ATTR_THUMB_IMAGE)));

    setVertical(rt.height() > rt.width());
    // SkinUtils::strToBool(element.attribute(ATTR_VERTICAL))
}
