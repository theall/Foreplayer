#include "tsliderstyle.h"
#include "tsliderbar.h"
#include "tscrollbar.h"

TSliderStyle::TSliderStyle():
    QCommonStyle()
{

}

int TSliderStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const{

    if(hint == QStyle::SH_Slider_AbsoluteSetButtons)
        return (Qt::LeftButton);
    return QCommonStyle::styleHint(hint, option, widget, returnData);
}

int TSliderStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const{
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

void TSliderStyle::polish(QWidget *widget){
    widget->setAttribute(Qt::WA_Hover, true);
}
void TSliderStyle::unpolish(QWidget *widget){
    widget->setAttribute(Qt::WA_Hover, false);
}

void TSliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_Slider){
        QRect grooveRect = subControlRect(CC_Slider, option, SC_SliderGroove, widget);
        QRect buttonRect = subControlRect(CC_Slider, option, SC_SliderHandle, widget);

        painter->save();

        if(const TSliderBar *sliderBar = qobject_cast<const TSliderBar *>(widget)){
            Qt::Orientation orientation = sliderBar->orientation();
            const QPixmap *groovePixmap = sliderBar->groovePixmap();
            const QPixmap *fillPixmap = sliderBar->fillPixmap();
            QRect sliderRect = sliderBar->rect();

            if(const QStyleOptionSlider *optionStyle = qstyleoption_cast<const QStyleOptionSlider *>(option)){
                if ((option->subControls & SC_SliderGroove) && grooveRect.isValid() && buttonRect.isValid()) {
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

                    if((optionStyle->state & QStyle::State_MouseOver))
                    {
                        if(optionStyle->state & State_Sunken)
                            drawPixmap = mouseDown;
                        else
                            drawPixmap = hover;
                    }else if(!(optionStyle->state & QStyle::State_Enabled))
                    {
                        // Diabled
                        drawPixmap = disabled;
                    }
                    if(!drawPixmap->isNull())
                    {
                        if(orientation == Qt::Vertical){
                            painter->drawPixmap((sliderRect.width() - drawPixmap->width()) / 2, buttonRect.y(), *drawPixmap);
                        }else if(orientation == Qt::Horizontal){
                            painter->drawPixmap(buttonRect.x(), (sliderRect.height() - drawPixmap->height()) / 2, *drawPixmap);
                        }
                    }
                }
            }
        }
        painter->restore();
    }else if(control == CC_ScrollBar){
        QRect grooveRect = subControlRect(CC_Slider, option, SC_SliderGroove, widget);
        QRect buttonRect = subControlRect(CC_Slider, option, SC_SliderHandle, widget);

        painter->save();

        if(const TScrollBar *scrollBar = qobject_cast<const TScrollBar *>(widget)){
            Qt::Orientation orientation = scrollBar->orientation();
            QRect sliderRect = scrollBar->rect();

            if(const QStyleOptionSlider *optionStyle = qstyleoption_cast<const QStyleOptionSlider *>(option)){
//                if ((option->subControls & SC_ScrollBarGroove) && grooveRect.isValid() && buttonRect.isValid()) {
//                    // background
//                    if(!groovePixmap->isNull())
//                        painter->drawPixmap(grooveRect, *groovePixmap);
//                }
                if(option->subControls & SC_ScrollBarSlider){
                    const TButtonPixmap *buttonPixmap = scrollBar->buttonPixmap();
                    const QPixmap *normal = buttonPixmap->normal();
                    const QPixmap *hover = buttonPixmap->hover();
                    const QPixmap *mouseDown = buttonPixmap->mouseDown();
                    const QPixmap *disabled = buttonPixmap->diabled();
                    const QPixmap *drawPixmap = normal;

                    if((optionStyle->state & QStyle::State_MouseOver))
                    {
                        if(optionStyle->state & State_Sunken)
                            drawPixmap = mouseDown;
                        else
                            drawPixmap = hover;
                    }else if(!(optionStyle->state & QStyle::State_Enabled))
                    {
                        // Diabled
                        drawPixmap = disabled;
                    }
                    if(!drawPixmap->isNull())
                    {
                        if(orientation == Qt::Vertical){
                            painter->drawPixmap((sliderRect.width() - drawPixmap->width()) / 2, buttonRect.y(), *drawPixmap);
                        }else if(orientation == Qt::Horizontal){
                            painter->drawPixmap(buttonRect.x(), (sliderRect.height() - drawPixmap->height()) / 2, *drawPixmap);
                        }
                    }
                }
            }
        }
        painter->restore();
    }
}
