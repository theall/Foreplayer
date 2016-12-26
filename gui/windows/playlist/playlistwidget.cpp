#include "playlistwidget.h"

TPlaylistWidget::TPlaylistWidget(QWidget *parent) :
    QWidget(parent),
    mMinWidth(100),
    mMinHeight(60),
    mMarginLeft(0),
    mMarginTop(0),
    mMarginRight(0),
    mMarginBottom(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    mPlaylistView = new TPlaylistView(this);
    mMusiclistView = new TMusiclistView(this);
    mTracklistView = new TTracklistView(this);

    connect(mPlaylistView, SIGNAL(onCurrentRowSelected(int)), this, SLOT(slotCurrentRowChanged(int)));
    connect(mMusiclistView, SIGNAL(onCurrentRowSelected(int)), this, SLOT(slotCurrentRowChanged(int)));
    connect(mTracklistView, SIGNAL(onCurrentRowSelected(int)), this, SLOT(slotCurrentRowChanged(int)));

    mSplitter = new TSplitter(this);

    mSplitter->addWidget(mPlaylistView);
    mSplitter->addWidget(mMusiclistView);
    mSplitter->addWidget(mTracklistView);

    mSplitter->setStretchFactor(0, 2);
    mSplitter->setStretchFactor(1, 4);
    mSplitter->setStretchFactor(2, 4);
    layout->addWidget(mSplitter);

    setLayout(layout);
}

void TPlaylistWidget::setAlignment(QPixmap background, QRect rect)
{
    mMarginLeft = rect.left();
    mMarginTop = rect.top();
    mMarginRight = background.width() - rect.right() + 1;
    mMarginBottom = background.height() - rect.bottom() + 1;
    mMinWidth = rect.width();
    mMinHeight = rect.height();
}

bool TPlaylistWidget::updatePosition(QSize size)
{
    int newRight = size.width() - mMarginRight;
    int newBottom = size.height() - mMarginBottom;
    int newWidth = newRight - mMarginLeft;
    int newHeight = newBottom - mMarginTop;
    QRect newGeometry(mMarginLeft, mMarginTop, newWidth, newHeight);
    bool canResize = false;
    if(newWidth >= mMinWidth)
    {
        newGeometry.setRight(newRight);
        canResize = true;
    }
    if(newHeight >= mMinHeight)
    {
        newGeometry.setBottom(newBottom);
        canResize = true;
    }

    setGeometry(newGeometry);

    return canResize;
}

//void TPlaylistWidget::setFontColors(QFont font,
//                                    QColor text,
//                                    QColor hilight,
//                                    QColor number,
//                                    QColor duration,
//                                    QColor select,
//                                    QColor background1,
//                                    QColor background2)
void TPlaylistWidget::setFontColors(QColor background2)
{
//    emit requestSetColors(font,
//                          text,
//                          hilight,
//                          number,
//                          duration,
//                          select,
//                          background1);

    TAbstractTableView::setBackgroundColor(background2);
}

void TPlaylistWidget::slotCurrentRowChanged(int index)
{
    QObject *sendObject = sender();
    if(dynamic_cast<TPlaylistView*>(sendObject)) {
        emit onPlaylistIndexChanged(index);
    } else if (dynamic_cast<TMusiclistView*>(sendObject)) {
        emit onMusiclistIndexChanged(index);
    } else if (dynamic_cast<TTracklistView*>(sendObject)) {
        emit onTracklistIndexChanged(index);
    }
}

void TPlaylistWidget::loadFromSkin(QDomElement element, TSkin *skin)
{
    QDomElement parentElement = element.parentNode().toElement();
    setAlignment(skin->findPixmap(parentElement.attribute(ATTR_IMAGE)), SkinUtils::extractGeometry(element));
}
