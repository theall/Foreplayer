#ifndef TSCROLLBARSTYLE_H
#define TSCROLLBARSTYLE_H

#include "abstractstyle.h"

class TScrollbarStyle : public TAbstractStyle
{
public:
    TScrollbarStyle();

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

    void drawWidget(const QWidget *widget, const QStyleOptionComplex *option, QPainter *painter) const Q_DECL_OVERRIDE;

};

#endif // TSCROLLBARSTYLE_H
