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

void TSliderBar::setPixmap(TSliderBar::TPixmapType type, QPixmap pixmap)
{
    if(type==PT_GROOVE)
        mGroovePixmap = pixmap;
    else if (type==PT_BUTTON)
        mButtonPixmap.setPixmap(pixmap);
    else if (type==PT_FILL)
        mFillPixmap = pixmap;
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

void TSliderBar::mouseMoveEvent(QMouseEvent *e)
{
    QSlider::mouseMoveEvent(e);

    // The slider bar will not send active sub control message, so force update.
    update();
}

void TSliderBar::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    QRect rt = SkinUtils::extractGeometry(element);
    setGeometry(rt);
    mGroovePixmap = skin->findPixmap(element.attribute(ATTR_BAR_IMAGE));
    mFillPixmap = skin->findPixmap(element.attribute(ATTR_FILL_IMAGE));
    mButtonPixmap.setPixmap(skin->findPixmap(element.attribute(ATTR_THUMB_IMAGE)));

    setVertical(rt.height() > rt.width());
    // SkinUtils::strToBool(element.attribute(ATTR_VERTICAL))
}
