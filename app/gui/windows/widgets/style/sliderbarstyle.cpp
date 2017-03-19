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

    if ((option->subControls & SC_SliderGroove)) {

        QRect grooveRect = subControlRect(CC_Slider, option, SC_SliderGroove, widget);

        // background
        if(!groovePixmap->isNull())
            painter->drawPixmap(grooveRect, *groovePixmap);

        if(!fillPixmap->isNull())
        {
            int minValue = sliderBar->minimum();
            int valueRange = sliderBar->maximum()-minValue;
            if(valueRange > 0)
            {
                float drawFactor = (float)(sliderBar->value()-minValue)/valueRange;
                int x = 0;
                int y = 0;
                int w = 0;
                int h = 0;
                QPixmap pixToDraw;
                if(orientation == Qt::Vertical) {
                    w = fillPixmap->width();
                    x = (float)(sliderRect.width() - w) / 2 + 0.5;
                    h = grooveRect.height()*drawFactor + 0.5;
                    y = grooveRect.bottom() - h + 1;
                    pixToDraw = fillPixmap->copy(0, y, w, h);
                } else if(orientation == Qt::Horizontal) {
                    h = fillPixmap->height();
                    y = (float)(sliderRect.height() - h) / 2 + 0.5;
                    w = grooveRect.width()*drawFactor + 0.5;
                    pixToDraw = fillPixmap->copy(0, 0, w, h);
                }
                painter->drawPixmap(x, y, w, h, pixToDraw);
            }
        }
    }
    if(option->subControls & SC_SliderHandle){
        QRect drawRect(buttonRect.x(), buttonRect.y(), sliderRect.width(), sliderRect.height());
        if(orientation == Qt::Vertical){
            drawRect.setLeft(0);
            drawRect.setWidth(sliderRect.width());
            drawRect.setHeight(buttonRect.height());
        }else if(orientation == Qt::Horizontal){
            drawRect.setTop(0);
            drawRect.setHeight(sliderRect.height());
            drawRect.setWidth(buttonRect.width());
        }

        QPoint pt = widget->mapFromGlobal(QCursor::pos());

        drawButton(painter,
                   sliderBar->buttonPixmap(),
                   drawRect,
                   option,
                   drawRect.contains(pt));
        }

    painter->restore();
}

void TSliderbarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_Slider){
        drawWidget(widget, option, painter);
    }
}
