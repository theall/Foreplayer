#ifndef TPLAYLISTWIDGET_H
#define TPLAYLISTWIDGET_H

#include "pch.h"

#include "../widgets/tsplitter.h"
#include "playlistview.h"
#include "musiclistview.h"
#include "tracklistview.h"

class TPlaylistWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TPlaylistWidget(QWidget *parent = 0);

signals:

public slots:

private:
    TPlaylistView *mPlaylistView;
    TMusiclistView *mMusiclistView;
    TTracklistView *mTracklistView;
    TSplitter *mSplitterLeft;
    TSplitter *mSplitterRight;
};

#endif // TPLAYLISTWIDGET_H
