#include "toolbar.h"

TToolBar::TToolBar(QWidget *parent) :
    QWidget(parent)
{
    for(int i=0;i<BUTTON_COUNT;i++)
    {
        TImageButton *button = new TImageButton(this);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(slotButtonClicked(bool)));
        mButtons[i] = button;
    }
    retranslateUi();
}

TToolBar::~TToolBar()
{
    for(int i=0;i<BUTTON_COUNT;i++)
    {
        delete mButtons[i];
    }
}

void TToolBar::setPixmaps(QPixmap normal, QPixmap hover, QColor transparentColor)
{
    int x = 0;
    int iconWidth, pixmapWidth, pixmapHeight;
    pixmapWidth = normal.width();
    pixmapHeight = normal.height();
    iconWidth = pixmapWidth / BUTTON_COUNT;
    setGeometry(0, 0, pixmapWidth, pixmapHeight);
    for(int i=0;i<BUTTON_COUNT;i++)
    {
        TImageButton *button = mButtons[i];
        QPixmap pixmap(iconWidth*4, pixmapHeight);
        pixmap.fill(transparentColor);
        QPainter painter(&pixmap);
        QPixmap normalPixmap = normal.copy(x, 0, iconWidth, pixmapHeight);
        painter.drawPixmap(0, 0, normalPixmap);
        if(hover.isNull())
            painter.drawPixmap(iconWidth, 0, normalPixmap);
        else
            painter.drawPixmap(iconWidth, 0, hover.copy(x, 0, iconWidth, pixmapHeight));
        painter.drawPixmap(iconWidth*2, 0, normalPixmap);
        painter.drawPixmap(iconWidth*3, 0, normalPixmap);
        painter.end();

        pixmap.setMask(pixmap.createMaskFromColor(transparentColor));
        button->setPixmapRect(pixmap, QRect(x, 0, iconWidth, pixmapHeight));

        x += iconWidth;
    }
}

Qt::Alignment TToolBar::alignment()
{
    return mAlignment;
}

void TToolBar::retranslateUi()
{
    mButtons[BTN_ADD]->setToolTip(tr("Add"));
    mButtons[BTN_DELETE]->setToolTip(tr("Delete"));
    mButtons[BTN_PLAYLIST]->setToolTip(tr("Playlist"));
    mButtons[BTN_SORT]->setToolTip(tr("Sort"));
    mButtons[BTN_FIND]->setToolTip(tr("Find"));
    mButtons[BTN_EDIT]->setToolTip(tr("Edit"));
    mButtons[BTN_MODE]->setToolTip(tr("Mode"));
}

void TToolBar::slotButtonClicked(bool checked)
{
    TImageButton *button = dynamic_cast<TImageButton*>(sender());
    if(button)
    {
        int btn = -1;
        for(int i=0;i<BUTTON_COUNT;i++)
        {
            if(mButtons[i]==button)
                btn = i;
        }
        if(btn >= 0)
            emit requestToggleContexMenu(BUTTON(btn), button->pos(), checked);
    }
}

void TToolBar::setAlignment(QPixmap pixmap, Qt::Alignment alignment)
{
    mAlignment = alignment;
    QWidget *parent = parentWidget();
    if(parent)
    {
        QRect geo = geometry();
        if(alignment & Qt::AlignHCenter)
            mAlignSize.setWidth(0);
        else if(alignment & Qt::AlignRight)
            mAlignSize.setWidth(pixmap.width() - geo.right());
        else
            mAlignSize.setWidth(geo.left());

        if(alignment & Qt::AlignVCenter)
            mAlignSize.setHeight(0);
        else if(alignment & Qt::AlignBottom)
            mAlignSize.setHeight(pixmap.height() - geo.bottom());
        else
            mAlignSize.setHeight(geo.top());

        updatePos();
    }
}

void TToolBar::uncheckButtons()
{
    for(int i=0;i<BUTTON_COUNT;i++)
    {
        mButtons[i]->setChecked(false);
    }
}

void TToolBar::updatePos()
{
    QWidget *parent = parentWidget();
    if(parent)
    {
        QRect rt = parent->geometry();
        QRect geo = geometry();
        int x = 0;
        int y = 0;
        if(mAlignment & Qt::AlignHCenter)
            x = (rt.width() - geo.width()) / 2;
        else if(mAlignment & Qt::AlignRight)
            x = rt.width() - mAlignSize.width() - geo.width();
        else
            x = mAlignSize.width();

        if(mAlignment & Qt::AlignVCenter)
            y = (rt.height() - geo.height()) / 2;
        else if(mAlignment & Qt::AlignBottom)
            y = rt.height() - mAlignSize.height() - geo.height();
        else
            y = mAlignSize.height();

        move(x, y);
    }
}

void TToolBar::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    setPixmaps(skin->findPixmap(element.attribute(ATTR_IMAGE)), skin->findPixmap(element.attribute(ATTR_HOT_IMAGE)), skin->transparentColor());
    setGeometry(SkinUtils::strToGeometry(element.attribute(ATTR_POSITION)));
    QString alignment = element.attribute(ATTR_ALIGN, "left+top");
    if(!alignment.isEmpty())
    {
        QDomElement parentElement = element.parentNode().toElement();
        setAlignment(skin->findPixmap(parentElement.attribute(ATTR_IMAGE)), SkinUtils::strToAlign(alignment));
    }
}
