#ifndef TSLIDERSTYLE_H
#define TSLIDERSTYLE_H

#include "tbuttonimage.h"

class TSliderStyle : public QCommonStyle
{
public:
    TSliderStyle();
    void setPixmaps(QPixmap groove, QPixmap button, QPixmap fill);
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const;
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
    void polish(QWidget *widget);
    void unpolish(QWidget *widget);
};
#endif // TSLIDERSTYLE_H
