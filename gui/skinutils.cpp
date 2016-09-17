#include "skinutils.h"

TPosition::TPosition()
{

}

TPosition::TPosition(QString s)
{
    setString(s);
}

void TPosition::setString(QString s)
{
    QStringList sl = s.split(",");
    if(sl.size()!=4)
    {
        return;
    }
    mGeometry.setLeft(sl[0].toInt());
    mGeometry.setTop(sl[1].toInt());
    mGeometry.setRight(sl[2].toInt());
    mGeometry.setBottom(sl[3].toInt());

    mRect.setLeft(0);
    mRect.setTop(0);
    mRect.setWidth(mGeometry.width());
    mRect.setHeight(mGeometry.height());
}

QRect TPosition::rect()
{
    return mRect;
}

QRect TPosition::geometry()
{
    return mGeometry;
}

bool str2bool(QString s)
{
    return false ? s.toLower()=="false" : true;
}

Qt::Alignment str2Align(QString s)
{
    Qt::Alignment i;
    s = s.toLower();
    if(s.indexOf("left")>=0)
        i |= Qt::AlignLeft;
    if(s.indexOf("right")>=0)
        i |= Qt::AlignRight;
    if(s.indexOf("bottom")>=0)
        i |= Qt::AlignBottom;
    if(s.indexOf("center")>=0)
        i |= Qt::AlignCenter;
    return i;
}
