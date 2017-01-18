#ifndef TIMAGEBUTTON_H
#define TIMAGEBUTTON_H

#include "buttonimage.h"
#include "../../share/skin.h"

class TImageButton : public QPushButton, TSkinReader
{
    Q_OBJECT

public:
    TImageButton(QWidget *parent=0);
    ~TImageButton();

    void setPixmapRect(QPixmap pixmap, QRect rect);
    void setPixmap(QPixmap pixmap);

    Qt::Alignment alignment();
    void setAlignment(QPixmap pixmap, Qt::Alignment alignment);

    void updatePos();

signals:
    void mouseEnter();
    void mouseLeave();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *eevent);
    void mouseReleaseEvent(QMouseEvent *eevent);

private:
    QSize mAlignSize;
    Qt::Alignment mAlignment;
    TButtonIcon *mButtonImages;

    void setIconAndSize(QIcon icon);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TIMAGEBUTTON_H
