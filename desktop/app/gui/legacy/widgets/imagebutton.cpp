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
#include "imagebutton.h"

TImageButton::TImageButton(QWidget *parent):
    QPushButton(parent),
    mButtonImages(new TButtonIcon)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setFlat(true);
    setFocusPolicy(Qt::NoFocus);
    setToolTipDuration(4000);
    setCursor(Qt::PointingHandCursor);
}

TImageButton::~TImageButton()
{
    delete mButtonImages;
}

void TImageButton::setPixmapRect(QPixmap pixmap, QRect rect)
{
    setGeometry(rect);

    mButtonImages->setPixmap(pixmap);

    setIconAndSize(*mButtonImages->normal());
}

void TImageButton::setPixmap(QPixmap pixmap)
{
    mButtonImages->setPixmap(pixmap);

    setIconAndSize(*mButtonImages->normal());
}

Qt::Alignment TImageButton::alignment()
{
    return mAlignment;
}

void TImageButton::setAlignment(QPixmap pixmap, Qt::Alignment alignment)
{
    mAlignment = alignment;
    QWidget *parent = parentWidget();
    if(parent)
    {
        QRect geo = geometry();
        if(alignment & Qt::AlignRight)
            mAlignSize.setWidth(pixmap.width() - geo.right());
        else if(mAlignment & Qt::AlignHCenter)
            mAlignSize.setWidth(0);
        else
            mAlignSize.setWidth(geo.left());

        if(alignment & Qt::AlignBottom)
            mAlignSize.setHeight(pixmap.height() - geo.bottom());
        else if(alignment & Qt::AlignVCenter)
            mAlignSize.setHeight(0);
        else
            mAlignSize.setHeight(geo.top());

        updatePos();
    }
}

void TImageButton::updatePos()
{
    QWidget *parent = parentWidget();
    if(parent)
    {
        QRect rt = parent->geometry();
        QRect geo = geometry();
        int x = 0;
        int y = 0;
        if(mAlignment & Qt::AlignRight)
            x = rt.width() - mAlignSize.width() - geo.width();
        else if(mAlignment & Qt::AlignHCenter)
            x = (rt.width() - geo.width()) / 2;
        else // Default align left
            x = mAlignSize.width();

        if(mAlignment & Qt::AlignBottom)
            y = rt.height() - mAlignSize.height() - geo.height();
        else if(mAlignment & Qt::AlignVCenter)
            y = (rt.height() - geo.height()) / 2;
        else // Default align top
            y = mAlignSize.height();

        move(x, y);

        move(x, y);
    }
}

void TImageButton::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);

    setIconAndSize(*mButtonImages->hover());

    emit mouseEnter();
}

void TImageButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);

    setIconAndSize(*mButtonImages->normal());

    emit mouseLeave();
}

void TImageButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        setIconAndSize(*mButtonImages->mouseDown());

    QPushButton::mousePressEvent(event);
}

void TImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);

    if(event->button()==Qt::LeftButton)
    {
        QPoint pt = event->pos();
        QRect rect(0, 0, width(), height());
        if(rect.contains(pt))
            setIconAndSize(*mButtonImages->hover());
        else
            setIconAndSize(*mButtonImages->normal());
    }
}

void TImageButton::setIconAndSize(QIcon icon)
{
    setIcon(icon);
    setIconSize(mButtonImages->size());
}


void TImageButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter p(this);
    QStyleOptionButton option;

    option.initFrom(this);
    option.features = QStyleOptionButton::None;
    option.features |= QStyleOptionButton::Flat;

    option.text = "";
    if(isChecked())
        option.icon = *mButtonImages->mouseDown();
    else
        option.icon = icon();
    option.iconSize = iconSize();

    p.drawControl(QStyle::CE_PushButton, option);
}

void TImageButton::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    setGeometry(SkinUtils::extractGeometry(element));
    mButtonImages->setPixmap(skin->findPixmap(element.attribute(ATTR_IMAGE)));
    setIconAndSize(*mButtonImages->normal());

    QString alignment = element.attribute(ATTR_ALIGN);
    if(!alignment.isEmpty())
    {
        QDomElement parentElement = element.parentNode().toElement();
        setAlignment(skin->findPixmap(parentElement.attribute(ATTR_IMAGE)), SkinUtils::strToAlign(alignment));
    }
}
