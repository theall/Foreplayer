#include "skinutils.h"

namespace SkinUtils {

    bool strToBool(QString s)
    {
        bool result = false;
        s = s.trimmed();
        if(s=="1" || s.toLower()=="true")
            result = true;

        return result;
    }

    Qt::Alignment strToAlign(QString s)
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

    QRect strToGeometry(QString s)
    {
        QRect rect;
        QStringList sl = s.split(",");
        if(sl.size()!=4)
        {
            return rect;
        }
        rect.setLeft(sl[0].toInt());
        rect.setTop(sl[1].toInt());
        rect.setRight(sl[2].toInt());
        rect.setBottom(sl[3].toInt());
        return rect;
    }

    QRect strToRect(QString s)
    {
        QRect rect;
        QStringList sl = s.split(",");
        if(sl.size()!=4)
        {
            return rect;
        }
        rect.setLeft(0);
        rect.setTop(0);
        rect.setWidth(sl[2].toInt()-sl[0].toInt());
        rect.setHeight(sl[3].toInt()-sl[1].toInt());
        return rect;
    }

    QRect extractGeometry(QDomElement e, QString attr)
    {
        return strToGeometry(e.attribute(attr));
    }

    QRect extractResizeRect(QDomElement e, QString attr)
    {
        return strToGeometry(e.attribute(attr));
    }

    Qt::Alignment extractAlignment(QDomElement e, QString attr)
    {
        return strToAlign(e.attribute(attr));
    }

    QFont extractFont(QDomElement e)
    {
        QFont ft;
        QString family = e.attribute(ATTR_FONT);
        if(!family.isEmpty())
            ft.setFamily(family);
        int fontSize = e.attribute(ATTR_FONT_SIZE).toInt();
        if(fontSize > 0)
            ft.setPixelSize(fontSize);
        return ft;
    }

} // namespace SkinUtils
