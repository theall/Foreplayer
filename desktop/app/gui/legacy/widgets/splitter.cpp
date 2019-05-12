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
#include "splitter.h"

#include <QPainter>
#include <QPolygonF>

#define HANDLE_BUTTON_HEIGHT 6
#define HANDLE_BUTTON_HEIGHT_HALF 3

TSplitterHandle::TSplitterHandle(int index, QSplitter *parent) :
    QSplitterHandle(Qt::Horizontal, parent),
    mIndex(index),
    mCollapsed(false)
{
    setMouseTracking(true);
}

void TSplitterHandle::setHandleStatus(bool collapsed)
{
    mCollapsed = collapsed;
    update();
}

bool TSplitterHandle::mouseInButton(int pos)
{
    QRect rt = rect();

    int height = rt.height()/2;
    if(pos>=height-HANDLE_BUTTON_HEIGHT_HALF && pos<=height+HANDLE_BUTTON_HEIGHT_HALF)
        return true;

    return false;
}

void TSplitterHandle::mouseMoveEvent(QMouseEvent *e)
{
    if(mouseInButton(e->pos().y()))
        setCursor(Qt::PointingHandCursor);
    else
    {
        setCursor(Qt::SizeHorCursor);
        QSplitterHandle::mouseMoveEvent(e);
    }
}

void TSplitterHandle::mouseReleaseEvent(QMouseEvent *e)
{
    if(mouseInButton(e->pos().y()))
    {
        mCollapsed = !mCollapsed;
        emit requestCollapse(mIndex, mCollapsed);
        update();
    }
    else
        QSplitterHandle::mouseReleaseEvent(e);
}

void TSplitterHandle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    QPolygonF triangle;
    QPainterPath trianglePath;
    QRect rt = rect();

    int left;
    int middle;
    int right;

    if((!mCollapsed && mIndex==2) || (mCollapsed && mIndex==1))
    {
        left = rt.left() + 1;
        middle = rt.top() + rt.height() / 2;
        right = rt.right() + 1;
    } else {
        left = rt.right() + 1;
        middle = rt.top() + rt.height() / 2;
        right = rt.left();
    }

    QColor fillColor("#6091c6");
    QColor borderColor = fillColor;
    QColor triangleColor = fillColor;
    fillColor.setAlpha(128);
    borderColor.setAlpha(196);

    // Draw border
    rt.adjust(0, 0, -1, -1);
    painter.setPen(borderColor);
    painter.drawRect(rt);

    // Fill rect
    rt.adjust(1, 1, 0, 0);
    painter.fillRect(rt, fillColor);

    // Draw triangle
    triangle.append(QPoint(left, middle-HANDLE_BUTTON_HEIGHT_HALF));
    triangle.append(QPoint(right, middle));
    triangle.append(QPoint(left, middle+HANDLE_BUTTON_HEIGHT_HALF));
    trianglePath.addPolygon(triangle);

    painter.fillPath(trianglePath, QBrush(triangleColor));
}

// ---------------------------------------------------------------------------------
TSplitter::TSplitter(QWidget *parent) :
    QSplitter(Qt::Horizontal, parent),
    mIndex(0)
{
    setHandleWidth(5);
    setOpaqueResize(true);
    setChildrenCollapsible(true);

    connect(this, SIGNAL(splitterMoved(int,int)), this, SLOT(slotSplitterMoved(int,int)));
}

void TSplitter::slotCollapseWidget(int index, bool collapse)
{
    QWidget *w = NULL;
    w = index==1 ? widget(0) : widget(mIndex-1);

    int newWidth = 0;
    if(collapse)
    {
        if(mWidgetWidth.contains(w))
            mWidgetWidth.remove(w);

        mWidgetWidth.insert(w, w->width());
    }
    else
    {
        QMap<QWidget*, int>::iterator i = mWidgetWidth.find(w);
        QRect rt = w->geometry();
        rt.setWidth(0);

        if(i!=mWidgetWidth.end())
            newWidth = i.value();
        else
            newWidth = 100;
    }
    QList<int> widgetsSize = sizes();
    if(widgetsSize.size() != 3)
        return;

    if(index==1)
        widgetsSize[0] = newWidth;
    else
        widgetsSize[2] = newWidth;

    setSizes(widgetsSize);
}

void TSplitter::slotSplitterMoved(int pos, int index)
{
    Q_UNUSED(pos)

    int w;
    QList<int> widgetsSize = sizes();

    if(index==1)
        w = widgetsSize[0];
    else
        w = widgetsSize[2];

    TSplitterHandle *h = dynamic_cast<TSplitterHandle*>(handle(index));
    if(h)
        h->setHandleStatus(w==0);
}

QSplitterHandle *TSplitter::createHandle()
{
    TSplitterHandle *handle = new TSplitterHandle(mIndex++, this);
    connect(handle, SIGNAL(requestCollapse(int, bool)), this, SLOT(slotCollapseWidget(int, bool)));
    return handle;
}

void TSplitterHandle::hideEvent(QHideEvent *e)
{
    e->ignore();
}

