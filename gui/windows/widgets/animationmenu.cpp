/*
 * Copyright (C) Kreogist Dev Team
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

#include "animationmenu.h"

TAnimationMenu::TAnimationMenu(QWidget *parent) :
    QMenu(parent),
    #ifndef Q_OS_MACX
    m_showContent(false),
    m_showAnime(new QPropertyAnimation(this, "geometry", this)),
    #endif
    m_mouseDownPos(QPoint(0,0))
{
    //Set properties.
    setAutoFillBackground(true);
    setWindowOpacity(0.85);
#ifndef Q_OS_MACX
    //Configure the animation.
    m_showAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_showAnime->setDuration(150);
    //When the show anime is finished, show the content.
    connect(m_showAnime, &QPropertyAnimation::finished,
            [=]
            {
                //Set the show content flag.
                m_showContent=true;
                //Update the menu.
                update();
            });
#endif
}

void TAnimationMenu::showEvent(QShowEvent *event)
{
#ifndef Q_OS_MACX
    //Hide the content.
    //m_showContent=false;
    //Configure the animation.
    //Prepare the position data.
    //Begin the height at 1px, the only reason is I don't want to see the '
    //invalid window content view size' warning under Mac OS X.
    //QRect endPosition=geometry(),
    //      startPosition=QRect(endPosition.x(),
    //                          m_mouseDownPos.y(),
    //                          1,
    //                          1);
    //Set the parameter to animation.
    //m_showAnime->setStartValue(startPosition);
    //m_showAnime->setEndValue(endPosition);
    //Start animation.
    //m_showAnime->start();
#endif
    //Do orignal show event.
    QMenu::showEvent(event);
}

void TAnimationMenu::paintEvent(QPaintEvent *event)
{
#ifndef Q_OS_MACX
    //Only do the original paint event when show content data is true.
    if(m_showContent)
    {
#endif
#ifdef Q_OS_MACX
        //Initial a painter.
        QPainter painter(this);
        //For Mac OS X, we have to force change the alpha of the back color to
        //be 255.
        QColor baseColor=palette().color(QPalette::Base);
        baseColor.setAlpha(255);
        painter.setBrush(baseColor);
        //Draw a border around the menu.
        painter.setPen(QColor(255,255,255,50));
        //Draw the base.
        painter.drawRect(0, 0, width()-1, height()-1);
#endif
        //Draw the menu content.
        QMenu::paintEvent(event);
#ifndef Q_OS_MACX
        //Initial a painter.
        QPainter painter(this);
        //Draw a border around the menu.
        painter.setPen(QColor(255,255,255,50));
        painter.drawRect(0, 0, width()-1, height()-1);
    }
#endif
}

void TAnimationMenu::setMouseDownPos(const QPoint &mouseDownPos)
{
    m_mouseDownPos = mouseDownPos;
}

void TAnimationMenu::setSeparatorColor(const QColor &color)
{
    //Set seperator style sheet, I can't solve this bug in coding way.
    setStyleSheet("QMenu::separator {height:1px;"
                  "background: rgba("+
                  QString::number(color.red())+
                  ", "+
                  QString::number(color.green())+
                  ", "+
                  QString::number(color.blue())+
                  ", "+
                  QString::number(color.alpha())+
                  ");"
                  "margin-left: 5px;"
                  "margin-right: 5px;}");
}
