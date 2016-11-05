#include "toolbar.h"

TToolBar::TToolBar(QWidget *parent) :
    QWidget(parent)
{
    for(int i=0;i<TOOL_BUTTON_COUNT;i++)
    {
        TImageButton *button = new TImageButton(this);
        button->setCheckable(true);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked(bool)));
        mButtons[i] = button;
    }
    retranslateUi();
}

TToolBar::~TToolBar()
{
    for(int i=0;i<TOOL_BUTTON_COUNT;i++)
    {
        delete mButtons[i];
    }
}

void TToolBar::setPixmaps(QPixmap normal, QPixmap hover)
{
    int x = 0;
    int iconWidth, pixmapWidth, pixmapHeight;
    pixmapWidth = normal.width();
    pixmapHeight = normal.height();
    iconWidth = pixmapWidth / TOOL_BUTTON_COUNT;

    setGeometry(0, 0, pixmapWidth, pixmapHeight);
    for(int i=0;i<TOOL_BUTTON_COUNT;i++)
    {
        TImageButton *button = mButtons[i];
        QPixmap pixmap(iconWidth*4, pixmapHeight);
        QPainter painter(&pixmap);
        QPixmap normalPixmap = normal.copy(x, 0, iconWidth, pixmapHeight);
        painter.drawPixmap(0, 0, normalPixmap);
        painter.drawPixmap(iconWidth, 0, hover.copy(x, 0, iconWidth, pixmapHeight));
        painter.drawPixmap(iconWidth*2, 0, normalPixmap);
        painter.drawPixmap(iconWidth*3, 0, normalPixmap);
        painter.end();

        button->setPixmapRect(pixmap, QRect(x, 0, iconWidth, pixmapHeight));

        x += iconWidth;
    }
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

void TToolBar::onButtonClicked(bool checked)
{
    for(int i=0;i<TOOL_BUTTON_COUNT;i++)
    {
        mButtons[i]->setChecked(checked);
    }
}

void TToolBar::setAlignment(QPixmap pixmap, Qt::Alignment alignment)
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

void TToolBar::updatePos()
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
        else if(mAlignment & Qt::AlignCenter)
            x = (rt.width() - geo.width()) / 2;

        if(mAlignment & Qt::AlignBottom)
            y = rt.height() - mAlignSize.height() - geo.height();

        move(x, y);
    }
}
