#include "sliderbar.h"

TSliderBar::TSliderBar(QWidget *parent) :
    QSlider(parent)
{
    initialize();
}

TSliderBar::TSliderBar(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent)
{
    initialize();
}

void TSliderBar::initialize()
{
    setCursor(Qt::PointingHandCursor);

    setRange(0, 100);

    setSingleStep(1);

    setMouseTracking(true);

    setStyle(&mStyle);
}

TSliderBar::~TSliderBar()
{
}

void TSliderBar::setPixmaps(QPixmap groove, QPixmap button, QPixmap fill)
{
    mGroovePixmap = groove;
    mFillPixmap = fill;
    mButtonPixmap.setPixmap(button);
}

void TSliderBar::setVertical(bool bVertical)
{
    setOrientation(bVertical ? Qt::Vertical : Qt::Horizontal);
}

const QPixmap *TSliderBar::groovePixmap() const
{
    return &mGroovePixmap;
}

const QPixmap *TSliderBar::fillPixmap() const
{
    return &mFillPixmap;
}

const TButtonPixmap *TSliderBar::buttonPixmap() const
{
    return &mButtonPixmap;
}
