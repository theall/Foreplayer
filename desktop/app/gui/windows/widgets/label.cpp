#include "label.h"

TLabel::TLabel(QWidget *parent):
    QLabel(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setScaledContents(true);
    setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
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
    if(!skin)
        return;

    setGeometry(SkinUtils::extractGeometry(element));

    QPixmap p = skin->findPixmap(element.attribute(ATTR_ICON));
    if(!p.isNull())
        setPixmap(p);

    setFontColor(SkinUtils::extractFont(element), QColor(element.attribute(ATTR_COLOR)));

    setAlignment(SkinUtils::extractAlignment(element));
}

void TLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        emit clicked();

    QLabel::mousePressEvent(event);
}
