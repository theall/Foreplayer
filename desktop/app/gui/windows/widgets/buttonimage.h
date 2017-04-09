#ifndef TBUTTONIMAGE_H
#define TBUTTONIMAGE_H

#include "pch.h"

enum TButtonIndex
{
    BI_Normal,
    BI_Hover,
    BI_MouseDown,
    BI_Disabled,
    TBUTTON_COUNT
};

class TButtonIcon
{
public:
    explicit TButtonIcon();

    void setPixmap(QPixmap pixmap, int count=TBUTTON_COUNT);
    void seIcon(TButtonIndex index, QIcon icon);

    const QIcon *normal() const;
    const QIcon *hover() const;
    const QIcon *mouseDown() const;
    const QIcon *diabled() const;

    QSize size();

private:
    QSize mSize;
    QVector<QIcon> mIcons;
};

class TButtonPixmap
{
public:
    explicit TButtonPixmap();

    void setPixmap(QPixmap pixmap, int count=TBUTTON_COUNT);
    void setPixmap(TButtonIndex index, QPixmap pixmap);

    const QPixmap *normal() const;
    const QPixmap *hover() const;
    const QPixmap *mouseDown() const;
    const QPixmap *diabled() const;

    const QSize size() const;

private:
    QSize mSize;
    QVector<QPixmap> mPixmaps;
};
#endif // TBUTTONIMAGE_H
