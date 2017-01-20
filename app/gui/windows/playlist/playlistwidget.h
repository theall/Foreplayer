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

    static void setFontColors(QColor background2);

    TPlaylistView *playlistView() { return mPlaylistView; }
    TMusiclistView *musiclistView() { return mMusiclistView; }
    TTracklistView *tracklistView() { return mTracklistView; }

    QFont textFont() { return mTextFont; }
    QColor colorText() { return mColorText; }
    QColor colorHilight() { return mColorHilight; }
    QColor colorBkgnd() { return mColorBkgnd; }
    QColor colorNumber() { return mColorNumber; }
    QColor colorDuration() { return mColorDuration; }
    QColor colorSelect() { return mColorSelect; }
    QColor colorBkgnd2() { return mColorBkgnd2; }

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

    QFont mTextFont;
    QColor mColorText;
    QColor mColorHilight;
    QColor mColorBkgnd;
    QColor mColorNumber;
    QColor mColorDuration;
    QColor mColorSelect;
    QColor mColorBkgnd2;

    // Cache the selected pixmap
    QPixmap mSelectedPixmap;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTWIDGET_H
