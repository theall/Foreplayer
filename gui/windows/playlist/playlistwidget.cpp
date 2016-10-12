#include "playlistwidget.h"

TPlaylistWidget::TPlaylistWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    mPlaylistView = new TPlaylistView(this);
    mMusiclistView = new TMusiclistView(this);
    mTracklistView = new TTracklistView(this);
    mSplitterLeft = new TSplitter(this);
    mSplitterRight = new TSplitter(this);

    layout->addWidget(mPlaylistView);
    layout->addWidget(mSplitterLeft);
    layout->addWidget(mMusiclistView);
    layout->addWidget(mSplitterRight);
    layout->addWidget(mTracklistView);

    setLayout(layout);
}

