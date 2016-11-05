#ifndef TPLAYLISTWIDGET_H
#define TPLAYLISTWIDGET_H

#include "../widgets/splitter.h"
#include "view/playlistview.h"
#include "view/musiclistview.h"
#include "view/tracklistview.h"
#include "model/playlistmodel.h"
#include "model/musiclistmodel.h"
#include "model/tracklistmodel.h"

class TPlaylistWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TPlaylistWidget(QWidget *parent = 0);

    /*
     * Set minimize size of this widget.
     */
    void setMinimizeRect(QRect rect);

    /*
     * Update this widget position in parent widget.
     * size: New size of parent widget
     * Return true if geometry updated.
     */
    bool updatePosition(QSize size);

    /*
     * Set text color
     */
    void setFontColors(QFont font,
                       QColor text,
                       QColor hilight,
                       QColor number,
                       QColor duration,
                       QColor select,
                       QColor background1,
                       QColor background2);

    /*
     * Set scroll bar pixmaps
     */
    void setScrollBarPixmaps(QPixmap groove, QPixmap button, QPixmap handle);

signals:

private slots:
    void slotCurrentRowChanged(int index);

private:
    TPlaylistView *mPlaylistView;
    TMusiclistView *mMusiclistView;
    TTracklistView *mTracklistView;
    TPlaylistModel *mPlaylistModel;
    TMusiclistModel *mMusiclistModel;
    TTrackListModel *mTracklistModel;

    TSplitter *mSplitter;
    QRect mMinimizeRect;
    int mLeft;
    int mTop;
    int mRight;
    int mBottom;
};

#endif // TPLAYLISTWIDGET_H
