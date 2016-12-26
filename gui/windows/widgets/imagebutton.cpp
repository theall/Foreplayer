#include "imagebutton.h"

TImageButton::TImageButton(QWidget *parent):
    QPushButton(parent),
    mButtonImages(new TButtonIcon)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setFlat(true);
    setFocusPolicy(Qt::NoFocus);
    setToolTipDuration(4000);
    setCursor(Qt::PointingHandCursor);
}

TImageButton::~TImageButton()
{
    delete mButtonImages;
}

void TImageButton::setPixmapRect(QPixmap pixmap, QRect rect)
{
    setGeometry(rect);

    mButtonImages->setPixmap(pixmap);

    setIconAndSize(*mButtonImages->normal());
}

void TImageButton::setPixmap(QPixmap pixmap)
{
    mButtonImages->setPixmap(pixmap);

    setIconAndSize(*mButtonImages->normal());
}

Qt::Alignment TImageButton::alignment()
{
    return mAlignment;
}

void TImageButton::setAlignment(QPixmap pixmap, Qt::Alignment alignment)
{
    mAlignment = alignment;
    QWidget *parent = parentWidget();
    if(parent)
    {
        QRect geo = geometry();
        if(alignment & Qt::AlignLeft)
            mAlignSize.setWidth(geo.left());
        else if(alignment & Qt::AlignRight)
            mAlignSize.setWidth(pixmap.width() - geo.right());
        else
            mAlignSize.setWidth(0);

        if(alignment & Qt::AlignTop)
            mAlignSize.setHeight(geo.top());
        else if(alignment & Qt::AlignBottom)
            mAlignSize.setHeight(pixmap.height() - geo.bottom());
        else
            mAlignSize.setHeight(0);

        updatePos();
    }
}

void TImageButton::updatePos()
{
    QWidget *parent = parentWidget();
    if(parent)
    {
        QRect rt = parent->geometry();
        QRect geo = geometry();
        int x = 0;
        int y = 0;
        if(mAlignment & Qt::AlignRight)
            x = rt.width() - mAlignSize.width() - geo.width();

        if(mAlignment & Qt::AlignBottom)
            y = rt.height() - mAlignSize.height() - geo.height();

        move(x, y);
    }
}

void TImageButton::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);

    setIconAndSize(*mButtonImages->hover());

    emit mouseEnter();
}

void TImageButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);

    setIconAndSize(*mButtonImages->normal());

    emit mouseLeave();
}

void TImageButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        setIconAndSize(*mButtonImages->mouseDown());

    QPushButton::mousePressEvent(event);
}

void TImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);

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

void TImageButton::setIconAndSize(QIcon icon)
{
    setIcon(icon);
    setIconSize(mButtonImages->size());
}


void TImageButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter p(this);
    QStyleOptionButton option;

    option.initFrom(this);
    option.features = QStyleOptionButton::None;
    option.features |= QStyleOptionButton::Flat;
    option.features |= QStyleOptionButton::HasMenu;

    option.text = "";
    if(isChecked())
        option.icon = *mButtonImages->mouseDown();
    else
        option.icon = icon();
    option.iconSize = iconSize();

    p.drawControl(QStyle::CE_PushButton, option);
}

void TImageButton::loadFromSkin(QDomElement element, TSkin *skin)
{
    setGeometry(SkinUtils::extractGeometry(element));
    mButtonImages->setPixmap(skin->findPixmap(element.attribute(ATTR_IMAGE)));
    setIconAndSize(*mButtonImages->normal());

    QString alignment = element.attribute(ATTR_ALIGN);
    if(!alignment.isEmpty())
    {
        QDomElement parentElement = element.parentNode().toElement();
        setAlignment(skin->findPixmap(parentElement.attribute(ATTR_IMAGE)), SkinUtils::strToAlign(alignment));
    }
}
