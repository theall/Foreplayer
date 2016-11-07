#include "sliderbarstyle.h"
#include "../sliderbar.h"

#include <QDebug>

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

    if ((option->subControls & SC_SliderGroove)) {

        QRect grooveRect = subControlRect(CC_Slider, option, SC_SliderGroove, widget);

        // background
        if(!groovePixmap->isNull())
            painter->drawPixmap(grooveRect, *groovePixmap);

        if(!fillPixmap->isNull())
        {
            if(orientation == Qt::Vertical)
            {
                painter->drawPixmap((sliderRect.width() - fillPixmap->width()) / 2, buttonRect.y(), fillPixmap->copy(0, buttonRect.y(), fillPixmap->width(), grooveRect.height() - buttonRect.y()));
            }
            else if(orientation == Qt::Horizontal)
            {
                painter->drawPixmap(0, (sliderRect.height() - fillPixmap->height()) / 2, fillPixmap->copy(0, 0, buttonRect.x() == 0 ? 1 : buttonRect.x(), fillPixmap->height()));
            }
        }
    }
    if(option->subControls & SC_SliderHandle){
        const TButtonPixmap *buttonPixmap = sliderBar->buttonPixmap();
        const QPixmap *normal = buttonPixmap->normal();
        const QPixmap *hover = buttonPixmap->hover();
        const QPixmap *mouseDown = buttonPixmap->mouseDown();
        const QPixmap *disabled = buttonPixmap->diabled();
        const QPixmap *drawPixmap = normal;

        if((option->state & QStyle::State_MouseOver))
        {
            if(option->state & State_Sunken)
                drawPixmap = mouseDown;
            else
                drawPixmap = hover;
        }else if(!(option->state & QStyle::State_Enabled))
        {
            // Diabled
            drawPixmap = disabled;
        }
        if(!drawPixmap->isNull())
        {

        }
        QRect drawRect(buttonRect.x(), buttonRect.y(), sliderRect.width(), sliderRect.height());

        if(orientation == Qt::Vertical){
            drawRect.setLeft(0);
            drawRect.setWidth(sliderRect.width());
            drawRect.setHeight(buttonRect.height());
            //painter->drawPixmap((sliderRect.width() - drawPixmap->width()) / 2, buttonRect.y(), *drawPixmap);
        }else if(orientation == Qt::Horizontal){
            drawRect.setTop(0);
            drawRect.setHeight(sliderRect.height());
            drawRect.setWidth(buttonRect.width());
            //painter->drawPixmap(buttonRect.x(), (sliderRect.height() - drawPixmap->height()) / 2, *drawPixmap);
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
