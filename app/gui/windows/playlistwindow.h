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
#include "playlist/menu/popmenumusiclistedit.h"

class TPlaylistWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TPlaylistWindow(QWidget *parent = 0);

    TPlaylistWidget *playlistWidget() { return mCentralWidget; }

signals:
    //// Controller
    // Play list
    void requestAddNewPlaylist();
    void requestRemovePlaylist();
    void requestRenamePlaylist();
    void requestSortPlaylists();
    void requestSendTo();

    // Music list
    // Remove musics
    void requestRemoveSelections(QList<int> indexes);
    void requestRemoveRedundant();
    void requestRemoveErrors();
    void requestRemoveAll();

    //// Gui
    void playlistWindowToggled(bool);

protected:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    // Widget
    void on_btnClose_clicked();

    // Gui control
    void slotRequestToggleButtonContexMenu(TToolBar::BUTTON id, QPoint pos, bool checked);
    void slotPopupContextMenu(QPoint pos);
    void slotOnMouseMove(QEvent *event);

    // Playlist view
    void slotActionRenameTriggered();

    // Add musics
    void slotOnActionAddMusicsTriggered();
    void slotOnActionAddDirectoryTriggered();

    // Remove musics
    void slotOnActionRemoveSelectionsTriggered();
    void slotOnActionRemoveRedundantTriggered();
    void slotOnActionRemoveErrorsTriggered();
    void slotOnActionRemoveAllTriggered();

private:
    TToolBar *mToolbar;
    TImageButton *mBtnClose;
    TPlaylistWidget *mCentralWidget;
    TPlaylistView *mPlaylistView;
    TMusiclistView *mMusiclistView;
    TTracklistView *mTracklistView;

    TPopMenuAddMusics *mPopmenuAddMusics;
    TPopMenuFind *mPopmenuFind;
    TPopMenuMusicList *mPopmenuMusicList;
    TPopMenuMusiclistEdit *mPopmenuMusiclistEdit;
    TPopMenuMusiclistItem *mPopmenuMusiclistItem;
    TPopMenuPlayList *mPopmenuPlayList;
    TPopMenuPlayMode *mPopmenuPlayMode;
    TPopMenuRemoveMusics *mPopmenuRemoveMusics;
    TPopMenuSort *mPopmenuSort;
    TPopMenuTrackList *mPopmenuTrackList;

    void tryAddMusicFiles(QStringList files);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTWINDOW_H
