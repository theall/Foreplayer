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
#ifndef TABSTRACTWINDOW_H
#define TABSTRACTWINDOW_H

#include "../share/skin.h"

enum TEdgeType
{
    ET_LEFT,
    ET_TOP,
    ET_RIGHT,
    ET_BOTTOM
};

enum TEdgeDirection
{
    ED_HORIZONTAL,
    ED_VERTICAL
};

#define EDGE_CHECK_WIDTH 10
#define INVALID_DISTANCE 0x7fffffff

class TEdge
{
public:
    int from;
    int to;
    int value;
    TEdgeType edgeType;
    TEdgeDirection direction;

    TEdge(int start, int end, int _value, TEdgeType _edgeType);

    // This distance is parallel distance between two lines which nearby in vertical direction
    int distanceTo(TEdge *edge);

    // This distance is parallel unity distance between two lines whatever where they are
    int parallelDistanceTo(TEdge *edge);

    bool isCloseTo(TEdge *edge, int distance=EDGE_CHECK_WIDTH);
    bool gumTo(TEdge *edge);
    void setValue(int start, int end, int _value);
};

struct TEdgePair
{
    TEdge *from;
    TEdge *to;
    int distance;
    TEdgePair(){
        from = NULL;
        to = NULL;
        distance = 0x7fffffff;
    }
    TEdgePair(TEdge *pFrom, TEdge *pTo, int pDistance){
        from = pFrom;
        to = pTo;
        distance = pDistance;
    }
};

class TBackgoundPixmap
{
public:
    TBackgoundPixmap();

    void draw(QPainter *painter, QRect rect);
    void setPixmap(QPixmap pixmap, QRect resize);

private:
    enum BlockNumber
    {
        BN_LEFT_TOP,
        BN_TOP,
        BN_RIGHT_TOP,
        BN_LEFT,
        BN_CENTER,
        BN_RIGHT,
        BN_LEFT_BOTTOM,
        BN_BOTTOM,
        BN_RIGHT_BOTTOM,
        BLOCK_COUNT
    };
    int mLeft;
    int mRight;
    int mTop;
    int mBottom;
    int mWidth;
    int mHeight;
    int mRigthWidth;
    int mBottomHeight;
    bool mResizable;
    QPixmap mPieces[BLOCK_COUNT];
    QPixmap mBackgound;
};

enum TResizeDirection {
    RD_NONE,
    RD_LEFTTOP,
    RD_UP,
    RD_RIGHTTOP,
    RD_RIGHT,
    RD_RIGHTBOTTOM,
    RD_DOWN,
    RD_LEFTBOTTOM,
    RD_LEFT,
    ResizeDirectionCount
};

class TAbstractWindow : public QMainWindow, TSkinReader
{
    Q_OBJECT
public:
    explicit TAbstractWindow(QWidget *parent = 0, bool resizeEnable = false);

    virtual void retranslateUi() = 0;
    bool mouseMoveTriggered();

    // Edges check
    QList<TEdge *> edges();

    bool isGumTo(TAbstractWindow *window);

    void setGeometry(QRect geometry);
    bool restoreGeometry(const QByteArray &geometry);
    bool restoreState(const QByteArray &state, int version = 0);

    void move(int x, int y);
    void move(const QPoint &);

signals:
    void requestMoveWindow(QPoint);
    void requestResizeWindow(QRect);
    void requestCloseWindow();
    void windowActived();

public slots:

private:
    bool mResizeEnable;
    bool mMousePressed;
    bool mMouseMoveTriggered;

    QPoint  mMousePos;
    QPoint  mWindowPos;
    QList<TEdge *> mEdges;
    TBackgoundPixmap mBackgound;
    TResizeDirection mResizingDirection;
    void updateEdges();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    bool event(QEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TABSTRACTWINDOW_H
