#ifndef TCHECKBOX_H
#define TCHECKBOX_H

#include "pch.h"
#include "tbuttonimage.h"

class TCheckBox : public QCheckBox
{
public:
    TCheckBox(QWidget *parent=0, TButtonType buttonType=BT_Normal);
    ~TCheckBox();

    void setPixmapRect(QPixmap pixmap, QRect rect);
    void setPixmap(QPixmap pixmap);
    void setFontColor(QFont font, QColor color);

protected:
    void enterEvent(QEvent *);//鼠标进入事件
    void leaveEvent(QEvent *);//离开
    void mousePressEvent(QMouseEvent *e);//点击
    void mouseReleaseEvent(QMouseEvent *e);//释放

private:
    TButtonIcon *m_buttonImages;

    void setIconAndSize(QIcon icon);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
};

#endif // TCHECKBOX_H
