#include "tscrollbar.h"

TScrollBar::TScrollBar(QWidget *parent) :
    QScrollBar(parent),
    mButtonPixmap(new TButtonPixmap(BT_NoDisable)),
    mHandlePixmap(new TButtonPixmap(BT_NoDisable))
{
    setOrientation(Qt::Horizontal);

    setCursor(Qt::PointingHandCursor);

    setRange(0, 100);

    setSingleStep(1);

    setMouseTracking(true);

    setAutoFillBackground(true);

    setStyle(&mStyle);
}

TScrollBar::~TScrollBar()
{
    delete mButtonPixmap;
    delete mHandlePixmap;
}

void TScrollBar::setPixmaps(QPixmap groove, QPixmap button, QPixmap handle)
{
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(groove));
    setPalette(palette);

    mButtonPixmap->setPixmap(button);
    mHandlePixmap->setPixmap(handle);
}

const TButtonPixmap *TScrollBar::handlePixmap() const
{
    return mHandlePixmap;
}

const TButtonPixmap *TScrollBar::buttonPixmap() const
{
    return mButtonPixmap;
}
