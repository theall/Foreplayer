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
#include "abstractwindow.h"

#include <QPainter>
#include <QMouseEvent>

#define RESIZE_CHECK_WIDTH 4

TEdge::TEdge(int start, int end, int _value, TEdgeType _edgeType)
{
    from = start;
    to = end;
    value = _value;
    edgeType = _edgeType;
    if(edgeType==ET_LEFT || edgeType==ET_RIGHT)
        direction = ED_VERTICAL;
    else
        direction = ED_HORIZONTAL;
}

int TEdge::distanceTo(TEdge *edge)
{
    if(direction != edge->direction)
        return INVALID_DISTANCE;

    if((from>=edge->from && from<=edge->to)
        || (to>=edge->from && to<=edge->to)
        || (edge->from>=from && edge->from<=to)
        || (edge->to>=from && edge->to<=to)
        )
        return value-edge->value;

    return INVALID_DISTANCE;
}

int TEdge::parallelDistanceTo(TEdge *edge)
{
    if(direction != edge->direction)
        return INVALID_DISTANCE;

    return value-edge->value;
}

bool TEdge::gumTo(TEdge *edge)
{
    if((value==edge->value) && ((from>=edge->from && from<=edge->to) || (to>=edge->from && to<=edge->to)))
        return true;
    return false;
}

void TEdge::setValue(int start, int end, int _value)
{
    from = start;
    to = end;
    value = _value;
}

const Qt::CursorShape C_RESIZE_ARROWS[ResizeDirectionCount] = {
    Qt::ArrowCursor,
    Qt::SizeFDiagCursor,
    Qt::SizeVerCursor,
    Qt::SizeBDiagCursor,
    Qt::SizeHorCursor,
    Qt::SizeFDiagCursor,
    Qt::SizeVerCursor,
    Qt::SizeBDiagCursor,
    Qt::SizeHorCursor
};

TAbstractWindow::TAbstractWindow(QWidget *parent, bool resizeEnable) : QMainWindow(parent)
    , mResizeEnable(resizeEnable)
    , mMousePressed(false)
    , mMouseMoveTriggered(false)
    , mResizingDirection(RD_NONE)
{
    setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);

    setMouseTracking(true);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);

    updateEdges();
}

bool TAbstractWindow::mouseMoveTriggered()
{
    return mMouseMoveTriggered;
}

void TAbstractWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isActiveWindow())
    {
        mWindowPos = pos();
        mMousePos = event->globalPos();
        mMousePressed = true;

        if(mResizingDirection != RD_NONE) {
            grabMouse();
        } else {
            mMouseMoveTriggered = true;
        }
    } else {
        QMainWindow::mousePressEvent(event);
    }
}

void TAbstractWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMousePressed = false;
        mMouseMoveTriggered = false;
        if(mResizingDirection != RD_NONE) {
            releaseMouse();
            setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void TAbstractWindow::mouseMoveEvent(QMouseEvent *event)
{
    // Get top left and bottom right of current rect
    QPoint gloabalPos = event->globalPos();
    QRect newRect = geometry();

    int left = newRect.left();
    int top = newRect.top();
    int right = newRect.right();
    int bottom = newRect.bottom();

    int leftEdge = left + RESIZE_CHECK_WIDTH;
    int topEdge = top + RESIZE_CHECK_WIDTH;
    int rightEdge = right - RESIZE_CHECK_WIDTH;
    int bottomEdge = bottom - RESIZE_CHECK_WIDTH;

    int x = gloabalPos.x();
    int y = gloabalPos.y();

//    qDebug("this:%d, down: %d, dir: %d, rect:%d %d %d %d, pos: %d %d",
//           this,
//           mMousePressed,
//           mResizingDirection,
//           left,
//           top,
//           right,
//           bottom,
//           x,
//           y);

    if (!mMousePressed)
    {
        if(mResizeEnable)
        {
            // Left
            if(x>=left && x<=leftEdge)
            {
                if(y>=top && y<=topEdge+RESIZE_CHECK_WIDTH)
                {
                    // Top left
                    mResizingDirection = RD_LEFTTOP;
                } else if(y>=bottomEdge-RESIZE_CHECK_WIDTH && y<=bottom) {
                    // Bottom left
                    mResizingDirection = RD_LEFTBOTTOM;
                } else {
                    mResizingDirection = RD_LEFT;
                }
            } else if(x>=rightEdge && x<=right) {
                // Right
                if(y>=top && y<=topEdge+RESIZE_CHECK_WIDTH)
                {
                    // Top left
                    mResizingDirection = RD_RIGHTTOP;
                } else if(y>=bottomEdge-RESIZE_CHECK_WIDTH && y<=bottom) {
                    // Right bottom
                    mResizingDirection = RD_RIGHTBOTTOM;
                } else {
                    mResizingDirection = RD_RIGHT;
                }
            } else if(y>=top && y<=topEdge) {
                if(x>=left && x<=leftEdge+RESIZE_CHECK_WIDTH)
                {
                    // Top left
                    mResizingDirection = RD_LEFTTOP;
                } else if(x>=rightEdge-RESIZE_CHECK_WIDTH && x<=right) {
                    // Right top
                    mResizingDirection = RD_RIGHTTOP;
                } else {
                    // Up
                    mResizingDirection = RD_UP;
                }
            } else if(y>=bottomEdge && y<=bottom) {
                if(x>=left && x<=leftEdge+RESIZE_CHECK_WIDTH)
                {
                    // Bottom left
                    mResizingDirection = RD_LEFTBOTTOM;
                } else if(x>=rightEdge-RESIZE_CHECK_WIDTH && x<=right) {
                    // Right top
                    mResizingDirection = RD_RIGHTBOTTOM;
                } else {
                    // Down
                    mResizingDirection = RD_DOWN;
                }
            } else {
                mResizingDirection = RD_NONE;
            }
        }
    } else {
        if(mResizingDirection != RD_NONE && mResizeEnable) {
            switch(mResizingDirection) {
                case RD_LEFT:
                    if(right - x <= this->minimumWidth())
                        newRect.setX(left);
                    else
                        newRect.setX(x);
                    break;
                case RD_RIGHT:
                    newRect.setWidth(x - left);
                    break;
                case RD_UP:
                    if(bottom - y <= this->minimumHeight())
                        newRect.setY(top);
                    else
                        newRect.setY(y);
                    break;
                case RD_DOWN:
                    newRect.setHeight(y - top);
                    break;
                case RD_LEFTTOP:
                    if(right - x <= this->minimumWidth())
                        newRect.setX(left);
                    else
                        newRect.setX(x);
                    if(bottom - y <= this->minimumHeight())
                        newRect.setY(top);
                    else
                        newRect.setY(y);
                    break;
                case RD_RIGHTTOP:
                    newRect.setWidth(x - left);
                    if(bottom - y <= minimumHeight())
                        newRect.setY(top);
                    else
                        newRect.setY(y);
                    break;
                case RD_LEFTBOTTOM:
                    newRect.setHeight(y - top);
                    if(right - x <= minimumWidth())
                        newRect.setX(left);
                    else
                        newRect.setX(x);
                    break;
                case RD_RIGHTBOTTOM:
                    newRect.setWidth(x - left);
                    newRect.setHeight(y - top);
                    break;
                default:
                    break;
            }
            emit requestResizeWindow(newRect);
        } else {
            emit requestMoveWindow(mWindowPos + (gloabalPos - mMousePos));
            mMouseMoveTriggered = false;

            event->accept();
        }
    }

    setCursor(C_RESIZE_ARROWS[mResizingDirection]);

    if(mResizingDirection == RD_NONE) {
        releaseMouse();
    } else {
        if(!isActiveWindow())
            activateWindow();

        // Here is to force receive mouse move event if there are child widgets
        //grabMouse();
    }
    QMainWindow::mouseMoveEvent(event);
}

bool TAbstractWindow::event(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        retranslateUi();
    }
    return QMainWindow::event(ev);
}

void TAbstractWindow::resizeEvent(QResizeEvent *ev)
{
    QMainWindow::resizeEvent(ev);

    updateEdges();
}

QList<TEdge *> TAbstractWindow::edges()
{
    return mEdges;
}

bool TAbstractWindow::isGumTo(TAbstractWindow *window)
{
    if(window==this)
        return false;
    QList<TEdge *> edges = window->edges();
    for(auto edge : mEdges)
    {
        for(auto e : edges)
        {
            if(edge->distanceTo(e)==0)
                return true;
        }
    }
    return false;
}

void TAbstractWindow::setGeometry(QRect geometry)
{
    QMainWindow::setGeometry(geometry);

    updateEdges();
}

bool TAbstractWindow::restoreGeometry(const QByteArray &data)
{
    QSizePolicy sp = sizePolicy();
    QRect oldGeo = geometry();
    bool status = QMainWindow::restoreGeometry(data);
    QRect newGeo = geometry();
    if(sp.horizontalPolicy()==QSizePolicy::Fixed)
        newGeo.setWidth(oldGeo.width());
    if(sp.verticalPolicy()==QSizePolicy::Fixed)
        newGeo.setHeight(oldGeo.height());
    setGeometry(newGeo);

    updateEdges();
    return status;
}

bool TAbstractWindow::restoreState(const QByteArray &state, int version)
{
    bool status = QMainWindow::restoreState(state, version);

    updateEdges();
    return status;
}

void TAbstractWindow::move(int x, int y)
{
    QMainWindow::move(x, y);

    updateEdges();
}

void TAbstractWindow::move(const QPoint &pos)
{
    QMainWindow::move(pos);

    updateEdges();
}

void TAbstractWindow::updateEdges()
{
    QRect newRect = geometry();
    int newLeft = newRect.left();
    int newRight = newRect.right();
    int newTop = newRect.top();
    int newBottom = newRect.bottom();

    if(mEdges.size()<=0)
    {
        mEdges.append(new TEdge(newTop, newBottom, newLeft, ET_LEFT));
        mEdges.append(new TEdge(newLeft, newRight, newTop, ET_TOP));
        mEdges.append(new TEdge(newTop, newBottom, newRight, ET_RIGHT));
        mEdges.append(new TEdge(newLeft, newRight, newBottom, ET_BOTTOM));
    }
    TEdge *edgeLeft = mEdges[ET_LEFT];
    TEdge *edgeTop = mEdges[ET_TOP];
    TEdge *edgeRight = mEdges[ET_RIGHT];
    TEdge *edgeBottom = mEdges[ET_BOTTOM];
    edgeLeft->setValue(newTop, newBottom, newLeft);
    edgeTop->setValue(newLeft, newRight, newTop);
    edgeRight->setValue(newTop, newBottom, newRight);
    edgeBottom->setValue(newLeft, newRight, newBottom);
}

void TAbstractWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    mBackgound.draw(&painter, rect());

    painter.end();
}

TBackgoundPixmap::TBackgoundPixmap()
{
    mLeft = 0;
    mRight = 0;
    mTop = 0;
    mBottom = 0;
    mWidth = 0;
    mHeight = 0;
    mRigthWidth = 0;
    mBottomHeight = 0;
    mResizable = false;
}

void TBackgoundPixmap::draw(QPainter *painter, QRect rect)
{
    if(mResizable)
    {
        int right = rect.right() - mRigthWidth;
        int bottom = rect.bottom() - mBottomHeight;
        int centerWidth = right - mLeft;
        int centerHeight = bottom - mTop;

        QPixmap *pix_left_top = &mPieces[BN_LEFT_TOP];
        QPixmap *pix_top = &mPieces[BN_TOP];
        QPixmap *pix_right_top = &mPieces[BN_RIGHT_TOP];
        QPixmap *pix_left = &mPieces[BN_LEFT];
        QPixmap *pix_center = &mPieces[BN_CENTER];
        QPixmap *pix_right = &mPieces[BN_RIGHT];
        QPixmap *pix_left_bottom = &mPieces[BN_LEFT_BOTTOM];
        QPixmap *pix_bottom = &mPieces[BN_BOTTOM];
        QPixmap *pix_right_bottom = &mPieces[BN_RIGHT_BOTTOM];

        // Block 1
        painter->drawPixmap(0, 0, *pix_left_top);

        // Block 2
        painter->drawPixmap(mLeft, 0, centerWidth, mTop, *pix_top);

        // Block 3
        painter->drawPixmap(right, 0, *pix_right_top);

        // Block 4
        painter->drawPixmap(0, mTop, mLeft, centerHeight, *pix_left);

        // Block 5
        painter->drawPixmap(mLeft, mTop, centerWidth, centerHeight, *pix_center);

        // Block 6
        painter->drawPixmap(right, mTop, mRigthWidth, centerHeight, *pix_right);

        // Block 7
        painter->drawPixmap(0, bottom, *pix_left_bottom);

        // Block 8
        painter->drawPixmap(mLeft, bottom, centerWidth, mBottomHeight, *pix_bottom);

        // Block 9
        painter->drawPixmap(right, bottom, *pix_right_bottom);
    } else {
        painter->drawPixmap(0, 0, mBackgound);
    }
}

void TBackgoundPixmap::setPixmap(QPixmap pixmap, QRect resize)
{
    if(resize.isValid())
    {
        mLeft = resize.left();
        mRight = resize.right();
        mTop = resize.top();
        mBottom = resize.bottom();
        mWidth = resize.width();
        mHeight = resize.height();
        mRigthWidth = pixmap.width() - mRight;
        mBottomHeight = pixmap.height() - mBottom;

        mPieces[BN_LEFT_TOP] = pixmap.copy(0, 0, mLeft, mTop);
        mPieces[BN_TOP] = pixmap.copy(mLeft, 0, mWidth, mTop);
        mPieces[BN_RIGHT_TOP] = pixmap.copy(mRight, 0, mRigthWidth, mTop);
        mPieces[BN_LEFT] = pixmap.copy(0, mTop, mLeft, mHeight);
        mPieces[BN_CENTER] = pixmap.copy(mLeft, mTop, mWidth, mHeight);
        mPieces[BN_RIGHT] = pixmap.copy(mRight, mTop, mRigthWidth, mHeight);
        mPieces[BN_LEFT_BOTTOM] = pixmap.copy(0, mBottom, mLeft, mBottomHeight);
        mPieces[BN_BOTTOM] = pixmap.copy(mLeft, mBottom, mWidth, mBottomHeight);
        mPieces[BN_RIGHT_BOTTOM] = pixmap.copy(mRight, mBottom, mRigthWidth, mBottomHeight);

        mResizable = true;
    }
    mBackgound = pixmap;
}


void TAbstractWindow::focusOutEvent(QFocusEvent *ev)
{
    mMousePressed = false;
    releaseMouse();
    QMainWindow::focusOutEvent(ev);
}

void TAbstractWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    QRect geo = SkinUtils::extractGeometry(element);
    QPixmap pixmap = skin->findPixmap(element.attribute(ATTR_IMAGE));
    setMinimumWidth(pixmap.width());
    setMinimumHeight(pixmap.height());

    if(!geo.isValid())
    {
        geo.setWidth(pixmap.width());
        geo.setHeight(pixmap.height());
    }
    setGeometry(geo);

    if(!pixmap.isNull())
    {
        mBackgound.setPixmap(pixmap, SkinUtils::extractResizeRect(element));
    }
}

void TAbstractWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);

    activateWindow();
    emit windowActived();
}

void TAbstractWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        if(!isActiveWindow())
        {
            mMousePressed = false;
            releaseMouse();
        }
    }
}

void TAbstractWindow::closeEvent(QCloseEvent *event)
{
    emit requestCloseWindow();
    event->ignore();
}
