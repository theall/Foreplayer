#include "checkbox.h"

TCheckBox::TCheckBox(QWidget *parent):
    QCheckBox(parent),
    mButtonImages(new TButtonIcon)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setFocusPolicy(Qt::NoFocus);
    setToolTipDuration(4000);
    setCursor(Qt::PointingHandCursor);
}

TCheckBox::~TCheckBox()
{
    delete mButtonImages;
}

void TCheckBox::setPixmapRect(QPixmap pixmap, QRect rect)
{
    setGeometry(rect);
    setPixmap(pixmap);
}

void TCheckBox::setPixmap(QPixmap pixmap)
{
    mButtonImages->setPixmap(pixmap);

    setIconAndSize(*mButtonImages->normal());
}

void TCheckBox::enterEvent(QEvent *event)
{
    QCheckBox::enterEvent(event);

    setIconAndSize(*mButtonImages->hover());
}

void TCheckBox::leaveEvent(QEvent *event)
{
    QCheckBox::leaveEvent(event);

    setIconAndSize(*mButtonImages->normal());
}

void TCheckBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        setIconAndSize(*mButtonImages->mouseDown());

    QCheckBox::mousePressEvent(event);
}

void TCheckBox::mouseReleaseEvent(QMouseEvent *event)
{
    QCheckBox::mouseReleaseEvent(event);

    if(event->button()==Qt::LeftButton)
    {
        QPoint pt = event->pos();
        QRect rect(0, 0, width(), height());
        if(rect.contains(pt))
            setIconAndSize(*mButtonImages->hover());
        else
            setIconAndSize(*mButtonImages->normal());
    }
}

void TCheckBox::setIconAndSize(QIcon icon)
{
    setIcon(icon);
    setIconSize(mButtonImages->size());
}

void TCheckBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter p(this);
    QStyleOptionButton option;

    option.initFrom(this);
    option.features = QStyleOptionButton::None;
    option.features |= QStyleOptionButton::Flat;
    option.features |= QStyleOptionButton::HasMenu;

    option.text = text();
    if(isChecked())
        option.icon = *mButtonImages->mouseDown();
    else
        option.icon = icon();
    option.iconSize = iconSize();

    p.drawControl(QStyle::CE_PushButton, option);
}

void TCheckBox::setFontColor(QFont font, QColor color)
{
    setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText, color);
    setPalette(pa);
}

void TCheckBox::loadFromSkin(QDomElement element, TSkin *skin)
{
    setGeometry(SkinUtils::strToGeometry(element.attribute(ATTR_POSITION)));
    mButtonImages->setPixmap(skin->findPixmap(element.attribute(ATTR_CKBOX_IMAGE)));
    setIconAndSize(*mButtonImages->normal());

    setFontColor(SkinUtils::extractFont(element), QColor(element.attribute(ATTR_COLOR)));

    int interval = element.attribute(ATTR_INTERVAL).toInt();
    Q_UNUSED(interval)

}
