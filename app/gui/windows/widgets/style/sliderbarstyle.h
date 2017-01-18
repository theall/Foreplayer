#ifndef TSLIDERBARSTYLE_H
#define TSLIDERBARSTYLE_H

#include "abstractstyle.h"

class TSliderbarStyle : public TAbstractStyle
{
public:
    TSliderbarStyle();
    void setPixmaps(QPixmap groove, QPixmap button, QPixmap fill);

    void drawComplexControl(ComplexControl control,
                            const QStyleOptionComplex *option,
                            QPainter *painter,
                            const QWidget *widget) const;
    int styleHint(StyleHint hint,
                  const QStyleOption *option,
                  const QWidget *widget,
                  QStyleHintReturn *returnData) const;
    int pixelMetric(PixelMetric metric,
                    const QStyleOption *option,
                    const QWidget *widget) const;
    void polish(QWidget *widget);
    void unpolish(QWidget *widget);

    void drawWidget(const QWidget *widget, const QStyleOptionComplex *option, QPainter *painter) const Q_DECL_OVERRIDE;

private:
};

#endif // TSLIDERBARSTYLE_H
