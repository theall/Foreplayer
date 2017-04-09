#ifndef TABSTRACTSTYLE_H
#define TABSTRACTSTYLE_H

#include "../buttonimage.h"

class TAbstractStyle : public QCommonStyle
{
public:
    TAbstractStyle();

    void polish(QWidget *widget);
    void unpolish(QWidget *widget);

    virtual void drawWidget(const QWidget *widget, const QStyleOptionComplex *option, QPainter *painter) const = 0;

    void drawButton(QPainter *painter,
                    const TButtonPixmap *buttonPixmap,
                    QRect buttonRect,
                    const QStyleOptionComplex *optionStyle,
                    bool isActived = false,
                    bool autofill = false) const;
};
#endif // TABSTRACTSTYLE_H
