#include "playlistwidget.h"

TPlaylistWidget::TPlaylistWidget(QWidget *parent) :
    QWidget(parent),
    mLeft(0),
    mTop(0),
    mRight(0),
    mBottom(0)
{
    setFontColors(QFont("-11,0,0,0,400,0,0,0,0,3,2,4,34,Tahoma"),
                  QColor("#6091c6"),
                  QColor("#e3ecf4"),
                  QColor("#aac5dd"),
                  QColor("#6898c4"),
                  QColor("#3c80c4"),
                  QColor("#000000"),
                  QColor("#0c1623"));

    setScrollBarPixmaps(
                QPixmap("z:/skins/fulkfour/b1.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_button.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_thumb.bmp")
                );

    QVBoxLayout *layout = new QVBoxLayout();
    mPlaylistView = new TPlaylistView(this);
    mMusiclistView = new TMusiclistView(this);
    mTracklistView = new TTracklistView(this);
    mPlaylistModel = new TPlaylistModel(this);
    mMusiclistModel = new TMusiclistModel(this);
    mTracklistModel = new TTrackListModel(this);
    connect(mPlaylistView, SIGNAL(onCurrentRowSelected(int)), this, SLOT(slotCurrentRowChanged(int)));
    connect(mMusiclistView, SIGNAL(onCurrentRowSelected(int)), this, SLOT(slotCurrentRowChanged(int)));
    connect(mTracklistView, SIGNAL(onCurrentRowSelected(int)), this, SLOT(slotCurrentRowChanged(int)));

    mSplitter = new TSplitter(this);

    mPlaylistView->setModel(mPlaylistModel);
    mMusiclistView->setModel(mMusiclistModel);
    mTracklistView->setModel(mTracklistModel);

    mSplitter->addWidget(mPlaylistView);
    mSplitter->addWidget(mMusiclistView);
    mSplitter->addWidget(mTracklistView);

    layout->addWidget(mSplitter);

    setLayout(layout);
}

void TPlaylistWidget::setMinimizeRect(QRect rect)
{
    mLeft = rect.left();
    mTop = rect.top();

    QRect rt = parentWidget()->rect();

    mRight = rt.width() - rect.right();
    mBottom = rt.height() - rect.bottom();

    mMinimizeRect = rect;
}

bool TPlaylistWidget::updatePosition(QSize size)
{
    int newRight = size.width() - mRight;
    int newBottom = size.height() - mBottom;
    int newWidth = newRight - mLeft;
    int newHeight = newBottom - mTop;
    QRect newGeometry = mMinimizeRect;
    bool canResize = false;
    if(newWidth > mMinimizeRect.width())
    {
        newGeometry.setRight(newRight);
        canResize = true;
    }
    if(newHeight > mMinimizeRect.height())
    {
        newGeometry.setBottom(newBottom);
        canResize = true;
    }

    setGeometry(newGeometry);

    return canResize;
}

void TPlaylistWidget::setFontColors(QFont font,
                                    QColor text,
                                    QColor hilight,
                                    QColor number,
                                    QColor duration,
                                    QColor select,
                                    QColor background1,
                                    QColor background2)
{
    TAbstractModel::setFont(font);
    TAbstractModel::setTextColor(text);
    TAbstractModel::setBackgroundColor(background1);
    TAbstractModel::setCurrentRowTextColor(hilight);

    TAbstractTableView::setBackgroundColor(background2);
}

void TPlaylistWidget::setScrollBarPixmaps(QPixmap groove, QPixmap button, QPixmap handle)
{
    TScrollBar::setPixmaps(groove, button, handle);
}

void TPlaylistWidget::slotCurrentRowChanged(int index)
{
    QObject *sendObject = sender();
    if(dynamic_cast<TPlaylistView*>(sendObject)) {
        mPlaylistModel->setCurrentIndex(index);
    } else if (dynamic_cast<TMusiclistView*>(sendObject)) {
        mMusiclistModel->setCurrentIndex(index);
    } else if (dynamic_cast<TTracklistView*>(sendObject)) {
        mTracklistModel->setCurrentIndex(index);
    }
}
