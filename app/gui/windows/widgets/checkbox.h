#ifndef TCHECKBOX_H
#define TCHECKBOX_H

#include "../../share/skin.h"

#include "buttonimage.h"

class TCheckBox : public QCheckBox, TSkinReader
{
public:
    TCheckBox(QWidget *parent=0);
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
    TButtonIcon *mButtonImages;

    void setIconAndSize(QIcon icon);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TCHECKBOX_H
