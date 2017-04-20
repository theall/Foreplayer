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
#include "scrolllabel.h"

static int c_offset_y = 1;

TScrollLabel::TScrollLabel(QWidget *parent) : QWidget(parent)
  , mUpdateInterval(40)
  , mSwitchInterval(3000)
  , mShuttleInterval(1500)
  , mSwitchOnClick(true)
  , mSwitchEnd(false)
  , mState(NONE)
  , mTimerID(-1)
  , mIdleTime(0)
{

}

void TScrollLabel::setStrings(QStringList strlist)
{
    mMutex.lock();
    mStringList.clear();
    mLineWidths.clear();
    for(QString s : strlist) {
        if(!s.trimmed().isEmpty())
        {
            mStringList.append(s);
            mLineWidths.append(0);
        }
    }
    if(mTimerID != -1)
    {
        killTimer(mTimerID);
        mTimerID = -1;
    }
    if(mStringList.count() > 0)
    {
        mOffsetX = 0;
        mOffsetY = 0;
        mState = LINE_DISPLAYING;
        mTimerID = startTimer(mUpdateInterval);
        setToolTip(mStringList.join("\n"));
        setToolTipDuration(3000);
    } else {
        setToolTipDuration(0);
    }
    mMutex.unlock();
}

void TScrollLabel::setFontColor(QFont font, QColor color)
{
    setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText, color);
    setPalette(pa);
    c_offset_y = (float)(height() - font.pixelSize())/2 + 0.5;
}

void TScrollLabel::setSwitchOnClick(bool enabled)
{
    mSwitchOnClick = enabled;
}

void TScrollLabel::setSpeed(int factor)
{
    mSwitchInterval /= factor;
    mShuttleInterval /= factor;
}

void TScrollLabel::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    mMutex.lock();
    int h = height();
    int currentLineIndex = -mOffsetY / h;
    if(currentLineIndex >= mLineWidths.size())
        currentLineIndex = mLineWidths.size() - 1;
    int currentLineWidth = mLineWidths[currentLineIndex];

    if(mState==LINE_DISPLAYING)
    {
        mIdleTime += mUpdateInterval;
        // Switch if current display text is emplty, or idle time is time out
        if(mIdleTime >= mSwitchInterval)
        {
            mIdleTime = 0;
            mState = LINE_SWITCHING;
        } else {
            if(currentLineWidth > width())
            {
                mIdleTime = 0;
                mState = SHUTTLE_WAIT_START;
            }
        }
    }
    else if(mState==LINE_SWITCHING)
    {
        mOffsetY--;
        if(mOffsetY <= -(currentLineIndex+1)*h)
        {
            if(mOffsetY <= -mStringList.size() * h)
                mOffsetY = 0;
            mState = LINE_DISPLAYING;
        }
    }
    else if(mState==SHUTTLE_WAIT_START)
    {
        mIdleTime += mUpdateInterval;
        if(mIdleTime >= mShuttleInterval)
        {
            mIdleTime = 0;
            mState = SHUTTLING_LEFT;
        }
    }
    else if(mState==SHUTTLING_LEFT)
    {
        mOffsetX -= 1;

        if(width()-mOffsetX >= currentLineWidth)
        {
            mState = SHUTTLE_PAUSED_IDLE;
        }
    }
    else if(mState==SHUTTLE_PAUSED_IDLE)
    {
        mIdleTime += mUpdateInterval;
        if(mIdleTime >= mShuttleInterval)
        {
            mIdleTime = 0;
            mState = SHUTTLING_RIGHT;
        }
    }
    else if(mState==SHUTTLING_RIGHT)
    {
        mOffsetX += 1;
        if(mOffsetX >= 0)
        {
            mOffsetX = 0;
            mState = SHUTTLE_END_IDLE;
        }
    }
    else if(mState==SHUTTLE_END_IDLE)
    {
        mIdleTime += mUpdateInterval;
        if(mIdleTime > mSwitchInterval)
        {
            mIdleTime = 0;
            mState = LINE_SWITCHING;
        }
    }

    update();
    mMutex.unlock();
}

void TScrollLabel::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if(mSwitchOnClick && event->button()==Qt::LeftButton)
    {
        mState = LINE_SWITCHING;
    }
}

void TScrollLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter p(this);

    int w = width();
    int h = height();
    for(int i=0;i<mStringList.size();i++)
    {
        QString text = mStringList[i];
        int lineWidth = mLineWidths[i];
        if(lineWidth <= 0)
        {
            QRectF needed;
            QRectF source(0, 0, w, h);
            needed = p.boundingRect(source, Qt::AlignLeft|Qt::AlignVCenter, text);

            lineWidth = (int)needed.width();
            mLineWidths[i] = lineWidth;
        }
        p.drawText(mOffsetX, mOffsetY+(i+1)*h-c_offset_y-1, text);
    }

    p.end();
}

void TScrollLabel::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    Q_UNUSED(skin)

    setGeometry(SkinUtils::extractGeometry(element));
    setFontColor(SkinUtils::extractFont(element), QColor(element.attribute(ATTR_COLOR)));
}
