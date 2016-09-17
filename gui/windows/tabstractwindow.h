#ifndef TABSTRACTWINDOW_H
#define TABSTRACTWINDOW_H

#include "pch.h"
#include "skinutils.h"

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

    int distanceTo(TEdge *edge);
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

class TAbstractWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TAbstractWindow(QWidget *parent = 0);

    void setWindowParam(WindowParam *param);

    virtual void retranslateUi() = 0;
    bool mouseMoveTriggered();

    // Edges check
    QList<TEdge *> edges();

    bool isGumTo(TAbstractWindow *window);

    void setGeometry(QRect geometry);

    void move(int x, int y);
    void move(const QPoint &);

signals:
    void requestMoveWindow(QPoint);
    void requestResizeWindow(QRect);

public slots:

private:
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
};





#endif // TABSTRACTWINDOW_H
