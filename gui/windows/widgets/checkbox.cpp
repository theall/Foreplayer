#include "checkbox.h"

TCheckBox::TCheckBox(QWidget *parent):
    QCheckBox(parent),
    m_buttonImages(new TButtonIcon)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setFocusPolicy(Qt::NoFocus);
    setToolTipDuration(4000);
    setCursor(Qt::PointingHandCursor);
}

TCheckBox::~TCheckBox()
{
    delete m_buttonImages;
}

void TCheckBox::setPixmapRect(QPixmap pixmap, QRect rect)
{
    setGeometry(rect);

    m_buttonImages->setPixmap(pixmap);

    setIconAndSize(*m_buttonImages->normal());
}

void TCheckBox::setPixmap(QPixmap pixmap)
{
    m_buttonImages->setPixmap(pixmap);

    setIconAndSize(*m_buttonImages->normal());
}

void TCheckBox::enterEvent(QEvent *event)
{
    QCheckBox::enterEvent(event);

    setIconAndSize(*m_buttonImages->hover());
}

void TCheckBox::leaveEvent(QEvent *event)
{
    QCheckBox::leaveEvent(event);

    setIconAndSize(*m_buttonImages->normal());
}

void TCheckBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        setIconAndSize(*m_buttonImages->mouseDown());

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
            setIconAndSize(*m_buttonImages->hover());
        else
            setIconAndSize(*m_buttonImages->normal());
    }
}

void TCheckBox::setIconAndSize(QIcon icon)
{
    setIcon(icon);
    setIconSize(m_buttonImages->size());
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
        option.icon = *m_buttonImages->mouseDown();
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

