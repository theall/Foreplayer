#ifndef TPLAYLISTWIDGET_H
#define TPLAYLISTWIDGET_H

#include "../../share/skin.h"
#include "../widgets/splitter.h"
#include "view/playlistview.h"
#include "view/musiclistview.h"
#include "view/tracklistview.h"

class TPlaylistWidget : public QWidget, TSkinReader
{
    Q_OBJECT
public:
    explicit TPlaylistWidget(QWidget *parent = 0);

    /*
     * Set minimize size of this widget.
     */
    void setAlignment(QPixmap background, QRect rect);

    /*
     * Update this widget position in parent widget.
     * size: New size of parent widget
     * Return true if geometry updated.
     */
    bool updatePosition(QSize size);

    /*
     * Set text color
     */
//    static void setFontColors(QFont font,
//                       QColor text,
//                       QColor hilight,
//                       QColor number,
//                       QColor duration,
//                       QColor select,
//                       QColor background1,
//                       QColor background2);
    static void setFontColors(QColor background2);

    TPlaylistView *playlistView() { return mPlaylistView;}
    TMusiclistView *musiclistView() { return mMusiclistView;}
    TTracklistView *tracklistView() { return mTracklistView;}

signals:
    void onPlaylistIndexChanged(int index);
    void onMusiclistIndexChanged(int index);
    void onTracklistIndexChanged(int index);
//    void requestSetColors(QFont font,
//                          QColor textColor,
//                          QColor selectedTextColor,
//                          QColor numberColor,
//                          QColor durationColor,
//                          QColor currentRowTextColor,
//                          QColor backgroundColor);

private slots:
    void slotCurrentRowChanged(int index);

private:
    TPlaylistView *mPlaylistView;
    TMusiclistView *mMusiclistView;
    TTracklistView *mTracklistView;

    TSplitter *mSplitter;
    int mMinWidth;
    int mMinHeight;
    int mMarginLeft;
    int mMarginTop;
    int mMarginRight;
    int mMarginBottom;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTWIDGET_H
