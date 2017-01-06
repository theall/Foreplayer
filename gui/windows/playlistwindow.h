#ifndef TPLAYLISTWINDOW_H
#define TPLAYLISTWINDOW_H

#include "widgets/imagebutton.h"
#include "widgets/toolbar.h"
#include "abstractwindow.h"
#include "playlist/playlistwidget.h"

#include "playlist/menu/popmenuaddmusics.h"
#include "playlist/menu/popmenufind.h"
#include "playlist/menu/popmenumusiclist.h"
#include "playlist/menu/popmenumusiclistitem.h"
#include "playlist/menu/popmenuplaylist.h"
#include "playlist/menu/popmenuplaymode.h"
#include "playlist/menu/popmenuremovemusics.h"
#include "playlist/menu/popmenusort.h"
#include "playlist/menu/popmenutracklist.h"

class TPlaylistWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TPlaylistWindow(QWidget *parent = 0);

    TPlaylistWidget *playlistWidget() { return mCentralWidget; }

signals:
    // Controller
    void requestAddNewPlaylist();
    void requestRemovePlaylist();
    void requestRenamePlaylist();
    void requestSortPlaylists();
    void requestSendTo();

    // Gui
    void playlistWindowToggled(bool);

public slots:

protected:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void on_btnClose_clicked();

    void slotToolbarClicked(TToolBar::BUTTON id, QPoint pos);
    void slotToolbarButtonMouseLeave(TToolBar::BUTTON id);
    void slotPopupContextMenu(QPoint pos);

private:
    TToolBar *mToolbar;
    TImageButton *mBtnClose;
    TPlaylistWidget *mCentralWidget;

    TPopMenuAddMusics *mPopmenuAddMusics;
    TPopMenuFind *mPopmenuFind;
    TPopMenuMusicList *mPopmenuMusicList;
    TPopMenuMusiclistItem *mPopmenuMusiclistItem;
    TPopMenuPlayList *mPopmenuPlayList;
    TPopMenuPlayMode *mPopmenuPlayMode;
    TPopMenuRemoveMusics *mPopmenuRemoveMusics;
    TPopMenuSort *mPopmenuSort;
    TPopMenuTrackList *mPopmenuTrackList;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTWINDOW_H
