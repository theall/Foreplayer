#include "ledwidget.h"

TLedWidget::TLedWidget(QWidget *parent) :
    QWidget(parent),
    mSeconds(0),
    mTotalSeconds(0),
    mShowRemain(false),
    mAlignment(Qt::AlignLeft)
{
    for(int i=0;i<6;i++)
        mPending.append(QPixmap());
    for(int i=0;i<10;i++)
        mNumbers.append(QPixmap());
}

void TLedWidget::setPixmap(QPixmap pixmap)
{
    int width, height;
    int nw;
    width = pixmap.width();
    height = pixmap.height();
    nw = width / 12;
    for(int i=0;i<10;i++)
    {
        mNumbers[i] = pixmap.copy(i*nw, 0, nw, height);
    }
    mQuote = pixmap.copy(10*nw, 0, nw, height);
    mMinus = pixmap.copy(11*nw, 0, nw, height);
    mPending[3] = mQuote;

    refresh();
}

void TLedWidget::setTime(int seconds, int total)
{
    mSeconds = seconds;
    mTotalSeconds = total;
    refresh();
}

void TLedWidget::setAlignment(Qt::Alignment alignment)
{
    mAlignment = alignment;
    refresh();
}

void TLedWidget::refresh()
{
    int seconds = mShowRemain ? mTotalSeconds-mSeconds : mSeconds;
    if(seconds < 0)
        seconds = 0;
    int m = seconds / 60;
    if(m>99)
        m = 99;
    int s = seconds % 60;

    int mh = m / 10;
    int ml = m % 10;
    int sh = s / 10;
    int sl = s % 10;
    mPending[0] = mShowRemain ? mMinus : QPixmap();
    mPending[1] = mNumbers[mh];
    mPending[2] = mNumbers[ml];
    mPending[4] = mNumbers[sh];
    mPending[5] = mNumbers[sl];

    update();
}

void TLedWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    int x = 0;
    QPainter p(this);

    // Count the total width of images to draw
    int totalWidth = 0;
    for(int i=0;i<mPending.count();i++)
    {
        totalWidth += mPending[i].width();
    }
    if(mAlignment & Qt::AlignRight)
    {
        x = width() - totalWidth;
    }
    else if (mAlignment & Qt::AlignCenter)
    {
        x = (width() - totalWidth) / 2;
    }
    for(int i=0;i<mPending.count();i++)
    {
        int _w = mPending[i].width();
        if(_w > 0)
        {
            p.drawPixmap(x, 0, mPending[i]);
            x += _w;
        }
    }
    p.end();
}


void TLedWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        mShowRemain = !mShowRemain;
        refresh();
    }

    QWidget::mousePressEvent(event);
}
