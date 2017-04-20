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
#include "scrollbar.h"

QPixmap *TScrollBar::mGroovePixmap = NULL;
TButtonPixmap *TScrollBar::mHandlePixmap = NULL;
TButtonPixmap *TScrollBar::mUpPixmap = NULL;
TButtonPixmap *TScrollBar::mDownPixmap = NULL;
int TScrollBar::mFixWidth = 13;

#define SCROLLBAR_STYLE_SHEET \
"QScrollBar:vertical {width: %1px;padding-top:0px;padding-bottom:0px;}" \
"QScrollBar::sub-line:vertical{height:%2px;width:%3px;}" \
"QScrollBar::add-line:vertical{height:%4px;width:%5px;}"

TScrollBar::TScrollBar(QWidget *parent) :
    QScrollBar(parent)
{
    connect(this, SIGNAL(fixedWidthChanged()), this, SLOT(slotFixedWidthChanged()));
    initialize();
}

TScrollBar::TScrollBar(Qt::Orientation orientation, QWidget *parent) :
    QScrollBar(orientation, parent)
{
    initialize();
}

void TScrollBar::initialize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);    
    setRange(0, 100);

    setSingleStep(1);

    setMouseTracking(true);

    setStyle(&mStyle);
}

TScrollBar::~TScrollBar()
{

}

void TScrollBar::setPixmaps(QPixmap groove, QPixmap up, QPixmap down, QPixmap handle)
{
    if(!TScrollBar::mGroovePixmap)
        TScrollBar::mGroovePixmap = new QPixmap(groove);

    if(!TScrollBar::mUpPixmap)
        TScrollBar::mUpPixmap = new TButtonPixmap;
    TScrollBar::mUpPixmap->setPixmap(up, 3);

    if(!TScrollBar::mDownPixmap)
        TScrollBar::mDownPixmap = new TButtonPixmap;
    TScrollBar::mDownPixmap->setPixmap(down, 3);

    if(!TScrollBar::mHandlePixmap)
        TScrollBar::mHandlePixmap = new TButtonPixmap;
    TScrollBar::mHandlePixmap->setPixmap(handle, 3);

    TScrollBar::mFixWidth = TScrollBar::mGroovePixmap->width();
}

void TScrollBar::setPixmaps(QPixmap groove, QPixmap buttons, QPixmap handle)
{
    if(!TScrollBar::mGroovePixmap)
        TScrollBar::mGroovePixmap = new QPixmap;
    *TScrollBar::mGroovePixmap = groove;

    if(!TScrollBar::mUpPixmap)
        TScrollBar::mUpPixmap = new TButtonPixmap;

    if(!TScrollBar::mDownPixmap)
        TScrollBar::mDownPixmap = new TButtonPixmap;

    if(!TScrollBar::mHandlePixmap)
        TScrollBar::mHandlePixmap = new TButtonPixmap;
    TScrollBar::mHandlePixmap->setPixmap(handle, 3);

    TScrollBar::mFixWidth = TScrollBar::mGroovePixmap->width();

    int w = buttons.width();
    int h = buttons.height();
    if(h<1)
        return;

    h /= 2;
    QPixmap up = buttons.copy(0, 0, w, h);
    QPixmap down = buttons.copy(0, h, w, h);

    TScrollBar::mUpPixmap->setPixmap(up, 3);
    TScrollBar::mDownPixmap->setPixmap(down, 3);
}

const TButtonPixmap *TScrollBar::handlePixmap() const
{
    return TScrollBar::mHandlePixmap;
}

const TButtonPixmap *TScrollBar::upPixmap() const
{
    return TScrollBar::mUpPixmap;
}

const TButtonPixmap *TScrollBar::downPixmap() const
{
    return TScrollBar::mDownPixmap;
}

const QPixmap *TScrollBar::groovePixmap() const
{
    return TScrollBar::mGroovePixmap;
}

void TScrollBar::showEvent(QShowEvent *e)
{
    emit onVisibleToggle(true);
    e->accept();

    QScrollBar::showEvent(e);
}

void TScrollBar::hideEvent(QHideEvent *e)
{
    emit onVisibleToggle(false);
    e->accept();

    QScrollBar::hideEvent(e);
}

void TScrollBar::slotFixedWidthChanged()
{
    setStyleSheet(QString(SCROLLBAR_STYLE_SHEET)
                     .arg(TScrollBar::mFixWidth)
                     .arg(TScrollBar::mUpPixmap->size().height())
                     .arg(TScrollBar::mUpPixmap->size().width())
                     .arg(TScrollBar::mDownPixmap->size().height())
                     .arg(TScrollBar::mDownPixmap->size().width())
                     );
}
