#ifndef TBUTTONIMAGE_H
#define TBUTTONIMAGE_H

#include "pch.h"

enum TButtonType
{
    BT_SingleNormal=1,
    BT_NormalHover,
    BT_NoDisable,
    BT_Normal
};

enum TButtonIndex
{
    BI_Normal,
    BI_Disabled,
    BI_MouseDown,
    BI_Hover
};

class TButtonIcon : public QObject
{
public:
    explicit TButtonIcon(TButtonType butonType=BT_Normal);

    void setPixmap(QPixmap pixmap);

    const QIcon *normal() const;
    const QIcon *hover() const;
    const QIcon *mouseDown() const;
    const QIcon *diabled() const;

    const QIcon *icon() const;
    QSize size();

private:
    QSize mSize;
    QVector<QIcon> mIcons;
    QIcon mUnity;
    TButtonType mButtonType;
};

class TButtonPixmap : public QObject
{
public:
    explicit TButtonPixmap(TButtonType butonType=BT_Normal);

    void setPixmap(QPixmap pixmap);

    const QPixmap *normal() const;
    const QPixmap *hover() const;
    const QPixmap *mouseDown() const;
    const QPixmap *diabled() const;

    QSize size();

private:
    QSize mSize;
    QVector<QPixmap> mPixmaps;
    TButtonType mButtonType;
};
#endif // TBUTTONIMAGE_H
