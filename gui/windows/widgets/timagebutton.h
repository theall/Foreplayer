#ifndef TIMAGEBUTTON_H
#define TIMAGEBUTTON_H

#include "pch.h"
#include "tbuttonimage.h"

class TImageButton : public QPushButton
{
public:
    TImageButton(QWidget *parent=0, TButtonType buttonType=BT_Normal);
    ~TImageButton();

    void setPixmapRect(QPixmap pixmap, QRect rect);
    void setPixmap(QPixmap pixmap);

    Qt::Alignment alignment();
    void setAlignment(QPixmap pixmap, Qt::Alignment alignment);

    void updatePos();

protected:
    void enterEvent(QEvent *);//鼠标进入事件
    void leaveEvent(QEvent *);//离开
    void mousePressEvent(QMouseEvent *e);//点击
    void mouseReleaseEvent(QMouseEvent *e);//释放

private:
    QSize mAlignSize;
    Qt::Alignment mAlignment;
    TButtonIcon *m_buttonImages;

    void setIconAndSize(QIcon icon);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
};

#endif // TIMAGEBUTTON_H
