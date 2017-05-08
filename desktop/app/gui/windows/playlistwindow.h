/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
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
    void requestSortPlaylists();
    void requestSendTo();

    //// Music list
    // Remove musics
    void requestRemoveSelections(QList<int> indexes);
    void requestRemoveRedundant();
    void requestRemoveErrors();
    void requestRemoveAll();

    // Music item popup menu
    void requestReparseMusicItem(int row);
    void requestPlayMusicItem(int row);
    void requestCopyMusicItem(QList<int> rows);
    void requestCutMusicItem(QList<int> rows);
    void requestPasteMusicItem(int pos);
    void requestDeleteMusicItem(QList<int> rows);
    void requestExplorerMusicItem(int row);
    void requestExportMusicItem(int row);
    void requestViewMusicItem(int row);

    // Sort music list
    void requestSortMusiclistAsName();
    void requestSortMusiclistAsArtist();
    void requestSortMusiclistAsAlbum();
    void requestSortMusiclistAsDuration();
    void requestSortMusiclistRandom();
    void requestSortMusiclistReverse();

    // Track item popup menu
    void requestPlayTrackItem(int row);
    void requestCopyTrackItem(QList<int> rows);
    void requestExportTrackItem(QList<int> rows);
    void requestViewTrackItem(int row);

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
    void slotActionRemovePlaylist();

    // Add musics
    void slotOnActionAddMusicsTriggered();
    void slotOnActionAddDirectoryTriggered();

    // Remove musics
    void slotOnActionRemoveSelectionsTriggered();
    void slotOnActionRemoveRedundantTriggered();
    void slotOnActionRemoveErrorsTriggered();
    void slotOnActionRemoveAllTriggered();

    // Music item popup menu
    void slotReparseMusicItemTriggered();
    void slotPlayMusicItemTriggered();
    void slotCopyMusicItemTriggered();
    void slotCutMusicItemTriggered();
    void slotPasteMusicItemTriggered();
    void slotDeleteMusicItemTriggered();
    void slotRenameMusicItemTriggered();
    void slotExplorerMusicItemTriggered();
    void slotExportMusicItemTriggered();
    void slotViewMusicItemTriggered();

    // Track list popup menu
    void slotPlayTrackItemTriggered();
    void slotCopyTrackItemTriggered();
    void slotChangeTrackItemTriggered();
    void slotExportTrackItemTriggered();
    void slotViewTrackItemTriggered();

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
    bool verified();

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTWINDOW_H
