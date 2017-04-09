#include "scrollbarstyle.h"
#include "../scrollBar.h"

TScrollbarStyle::TScrollbarStyle():
    TAbstractStyle()
{

}

int TScrollbarStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    return QCommonStyle::styleHint(hint, option, widget, returnData);
}

int TScrollbarStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const{
    return QCommonStyle::pixelMetric(metric, option, widget);
}

void TScrollbarStyle::drawWidget(
        const QWidget *widget,
        const QStyleOptionComplex *option,
        QPainter *painter) const
{
    const TScrollBar *scrollBar = dynamic_cast<const TScrollBar *>(widget);
    const QStyleOptionSlider *scrollbarOption = qstyleoption_cast<const QStyleOptionSlider *>(option);

    if(!scrollBar || !scrollbarOption)
        return;

    bool maxedOut = (scrollbarOption->maximum == scrollbarOption->minimum);

    if(maxedOut)
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Groove pixmap
    QRect drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, scrollBar);
    painter->drawPixmap(scrollBar->rect(), *scrollBar->groovePixmap());

    const TButtonPixmap *buttonPixmap = NULL;
    bool isActived = false;
    if(option->subControls & SC_ScrollBarSlider)
    {
        drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, scrollBar);
        buttonPixmap = scrollBar->handlePixmap();
//        float factor = (float)scrollBar->value() / scrollBar->maximum();
//        int totalHeight = scrollBar->height() - scrollBar->downPixmap()->size().height() - scrollBar->upPixmap()->size().height() - buttonPixmap->size().height();
//        int posY = factor * totalHeight + 0.5;
//        drawRect.setTop(posY);
//        drawRect.setBottom(drawRect.top()+buttonPixmap->size().height());
        isActived = option->activeSubControls&SC_ScrollBarSlider;
        drawButton(painter,
                   buttonPixmap,
                   drawRect,
                   option,
                   isActived, true);
    }
    if(option->subControls & SC_ScrollBarAddLine)
    {
        drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarAddLine, scrollBar);
        buttonPixmap = scrollBar->downPixmap();
        //drawRect.setTop(drawRect.bottom() - buttonPixmap->size().height());
        isActived = option->activeSubControls&SC_ScrollBarAddLine;
        drawButton(painter,
                   buttonPixmap,
                   drawRect,
                   option,
                   isActived);
    }
    if(option->subControls & SC_ScrollBarSubLine)
    {
        drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSubLine, scrollBar);
        buttonPixmap = scrollBar->upPixmap();
        //drawRect.setBottom(drawRect.top() + buttonPixmap->size().height());
        isActived = option->activeSubControls&SC_ScrollBarSubLine;
        drawButton(painter,
                   buttonPixmap,
                   drawRect,
                   option,
                   isActived);
    }
    painter->restore();
}

void TScrollbarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_ScrollBar) {
        drawWidget(widget, option, painter);
    }
}
