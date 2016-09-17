#include "tbuttonimage.h"

/**
 * @brief TButtonIcon::TButtonIcon
 * @param parent
 */
TButtonIcon::TButtonIcon(TButtonType buttonType) :
    mButtonType(buttonType)
{

}

void TButtonIcon::setPixmap(QPixmap pixmap)
{
    int x, y;
    int width, uwidth;
    int height;
    width = pixmap.width();
    height = pixmap.height();
    uwidth = width / 4;

    x = 0;
    y = 0;

    mIcons.clear();
    for(int i=0;i<mButtonType;i++)
    {
        QPixmap p = pixmap.copy(x, y, uwidth, height);
        mUnity.addPixmap(p, QIcon::Mode(i));
        mIcons.append(QIcon(p));
        x += uwidth;
    }

    mSize.setWidth(uwidth);
    mSize.setHeight(height);
}

const QIcon *TButtonIcon::normal() const
{
    return &mIcons[BI_Normal];
}

const QIcon *TButtonIcon::hover() const
{
    if(mIcons.size()<2)
        return &mIcons[BI_Normal];
    return &mIcons[BI_Hover];
}

const QIcon *TButtonIcon::mouseDown() const
{
    if(mIcons.size()<3)
        return &mIcons[BI_Normal];
    return &mIcons[BI_MouseDown];
}

const QIcon *TButtonIcon::diabled() const
{
    if(mIcons.size()<4)
        return &mIcons[BI_Normal];
    return &mIcons[BI_Disabled];
}

const QIcon *TButtonIcon::icon() const
{
    return &mUnity;
}

QSize TButtonIcon::size()
{
    return mSize;
}


/**
 * @brief TButtonPixmap::TButtonPixmap
 * @param parent
 */
TButtonPixmap::TButtonPixmap(TButtonType buttonType) :
    mButtonType(buttonType)
{

}

void TButtonPixmap::setPixmap(QPixmap pixmap)
{
    int x, y;
    int width, uwidth;
    int height;
    width = pixmap.width();
    height = pixmap.height();
    uwidth = width / mButtonType;

    x = 0;
    y = 0;

    mPixmaps.clear();
    for(int i=0;i<mButtonType;i++)
    {
        mPixmaps.append(pixmap.copy(x, y, uwidth, height));
        x += uwidth;
    }

    mSize.setWidth(uwidth);
    mSize.setHeight(height);
}

const QPixmap *TButtonPixmap::normal() const
{
    return &mPixmaps[BI_Normal];
}

const QPixmap *TButtonPixmap::hover() const
{
    if(mPixmaps.size()<2)
        return &mPixmaps[BI_Normal];
    return &mPixmaps[BI_Hover];
}

const QPixmap *TButtonPixmap::mouseDown() const
{
    if(mPixmaps.size()<3)
        return &mPixmaps[BI_Normal];
    return &mPixmaps[BI_MouseDown];
}

const QPixmap *TButtonPixmap::diabled() const
{
    if(mPixmaps.size()<4)
        return &mPixmaps[BI_Normal];
    return &mPixmaps[BI_Disabled];
}

QSize TButtonPixmap::size()
{
    return mSize;
}
