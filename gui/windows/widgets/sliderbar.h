#ifndef TSLIDERBAR_H
#define TSLIDERBAR_H

#include "style/sliderbarstyle.h"

class TSliderBar : public QSlider
{
    Q_OBJECT
public:
    explicit TSliderBar(QWidget *parent = 0);
    explicit TSliderBar(Qt::Orientation orientation, QWidget *parent = 0);
    ~TSliderBar();

    void setPixmaps(QPixmap groove, QPixmap button, QPixmap fill);

    void setVertical(bool bVertical = false);

    const QPixmap *groovePixmap() const;
    const QPixmap *fillPixmap() const;
    const TButtonPixmap *buttonPixmap() const;

signals:

public slots:

private:
    QPixmap mGroovePixmap;
    QPixmap mFillPixmap;
    TButtonPixmap mButtonPixmap;
    TSliderbarStyle mStyle;
    void initialize();

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
};

#endif // TSLIDERBAR_H
