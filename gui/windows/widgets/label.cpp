#include "label.h"

TLabel::TLabel(QWidget *parent):
    QLabel(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setScaledContents(true);
}

void TLabel::setFontColor(QFont font, QColor color)
{
    setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText, color);
    setPalette(pa);
}

void TLabel::loadFromSkin(QDomElement element, TSkin *skin)
{
    setGeometry(SkinUtils::extractGeometry(element));

    QPixmap p = skin->findPixmap(element.attribute(ATTR_ICON));
    if(!p.isNull())
        setPixmap(p);

    setFontColor(SkinUtils::extractFont(element), QColor(element.attribute(ATTR_COLOR)));

    setAlignment(SkinUtils::extractAlignment(element));
}
