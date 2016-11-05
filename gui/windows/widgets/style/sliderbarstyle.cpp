#include "sliderbarstyle.h"
#include "../sliderbar.h"

TSliderbarStyle::TSliderbarStyle():
    TAbstractStyle()
{

}

int TSliderbarStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const{

    if(hint == QStyle::SH_Slider_AbsoluteSetButtons)
        return (Qt::LeftButton);
    return QCommonStyle::styleHint(hint, option, widget, returnData);
}

int TSliderbarStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const{
    // 对Slidr上的handle而言返回的是 横向 宽度;纵向 高度;
    if(const TSliderBar *sliderBar = qobject_cast<const TSliderBar *>(widget)){
        Qt::Orientation orientation = sliderBar->orientation();
        const QPixmap *normal = sliderBar->buttonPixmap()->normal();
        if(normal->isNull())
        {
            return 1;
        }
        if(orientation==Qt::Vertical)
        {
            return normal->height();
        }
        else if(orientation==Qt::Horizontal)
        {
            return normal->width();
        }
    }
    return QCommonStyle::pixelMetric(metric, option, widget);
}

void TSliderbarStyle::polish(QWidget *widget){
    widget->setAttribute(Qt::WA_Hover, true);
}
void TSliderbarStyle::unpolish(QWidget *widget){
    widget->setAttribute(Qt::WA_Hover, false);
}

void TSliderbarStyle::drawWidget(const QWidget *widget, const QStyleOptionComplex *option, QPainter *painter) const
{
    const TSliderBar *sliderBar = dynamic_cast<const TSliderBar *>(widget);
    if(!sliderBar)
        return;

    painter->save();

    Qt::Orientation orientation = sliderBar->orientation();
    const QPixmap *groovePixmap = sliderBar->groovePixmap();
    const QPixmap *fillPixmap = sliderBar->fillPixmap();
    QRect sliderRect = sliderBar->rect();
    QRect buttonRect = subControlRect(CC_Slider, option, SC_SliderHandle, widget);

    if (option->subControls & SC_SliderGroove)
    {
        QRect grooveRect = subControlRect(CC_Slider, option, SC_SliderGroove, widget);

        // background
        if(!groovePixmap->isNull())
            painter->drawPixmap(grooveRect, *groovePixmap);

        if(!fillPixmap->isNull())
        {
            if(orientation == Qt::Vertical)
            {
                painter->drawPixmap(0, 0, sliderRect.width(), sliderRect.height()-buttonRect.bottom(), *fillPixmap);
            }
            else if(orientation == Qt::Horizontal)
            {
                painter->drawPixmap(0, 0, buttonRect.x(), sliderRect.height(), *fillPixmap);
            }
        }
    }
    if(option->subControls&SC_SliderHandle)
    {
        drawButton(painter,
                   sliderBar->buttonPixmap(),
                   buttonRect,
                   option,
                   option->activeSubControls&SC_SliderHandle);
    }

    painter->restore();
}

void TSliderbarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_Slider){
        drawWidget(widget, option, painter);
    }
}
