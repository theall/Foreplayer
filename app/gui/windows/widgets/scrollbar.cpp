#include "scrollbar.h"

QPixmap *TScrollBar::mGroovePixmap = NULL;
TButtonPixmap *TScrollBar::mHandlePixmap = NULL;
TButtonPixmap *TScrollBar::mUpPixmap = NULL;
TButtonPixmap *TScrollBar::mDownPixmap = NULL;
int TScrollBar::mFixWidth = 13;

TScrollBar::TScrollBar(QWidget *parent) :
    QScrollBar(parent)
{
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

    setStyleSheet(QString("QScrollBar:vertical {width: %1px;}").arg(TScrollBar::mFixWidth));

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
