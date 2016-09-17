#ifndef TSCROLLBAR_H
#define TSCROLLBAR_H

#include "pch.h"
#include "tsliderstyle.h"

class TScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit TScrollBar(QWidget *parent = 0);
    ~TScrollBar();

    void setPixmaps(QPixmap groove, QPixmap button, QPixmap handle);

    const TButtonPixmap *handlePixmap() const;
    const TButtonPixmap *buttonPixmap() const;

signals:

public slots:

private:
    TButtonPixmap *mButtonPixmap;
    TButtonPixmap *mHandlePixmap;
    TSliderStyle mStyle;

};
#endif // TSCROLLBAR_H
