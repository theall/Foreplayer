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
#ifndef TPLAYLISTCONTROLLER_H
#define TPLAYLISTCONTROLLER_H

#include "abstractcontroller.h"

#include "model/playlistmodel.h"
#include "model/musiclistmodel.h"
#include "model/tracklistmodel.h"
#include "model/missionsmodel.h"

#include <QMutex>
#include <QSharedMemory>

class TPlaylistController : public TAbstractController
{
    Q_OBJECT

public:
    explicit TPlaylistController(QObject *parent = 0);
    ~TPlaylistController();

    bool joint(TGuiManager *manager, TCore *core) Q_DECL_OVERRIDE;

signals:
    void requestPlay(int pIndex, int mIndex, int tIndex);

private slots:
    //// From gui
    void slotSkinChanged();
    void slotPlaylistIndexChanged(int index);
    void slotMusiclistIndexChanged(int index);
    void slotTracklistIndexChanged(int index);

    void slotPlaylistItemSelected(int index);
    void slotMusiclistItemSelected(int index);
    void slotTracklistItemSelected(int index);

    void slotRequestMovePlaylists(QList<int> indexes, int pos, QList<int> &newIndexes);
    void slotRequestMoveMusics(QList<int> indexes, int pos, QList<int> &newIndexes);
    void slotRequestMoveTracks(QList<int> indexes, int pos, QList<int> &newIndexes);

    //// Play list
    void slotRequestAddNewPlaylist();
    void slotRequestRemovePlaylist();
    void slotRequestSortPlaylists();
    void slotRequestSendTo();

    //// Music list
    void slotRequestAddMusicFiles(QStringList files, int pos, QList<int> &newIndexes);
    void slotRequestDeleteMusicItem(QList<int> indexes);
    void slotRequestRemoveRedundant();
    void slotRequestRemoveErrors();
    void slotRequestRemoveAll();

    // Music item
    void slotRequestReparseMusicItem(int row);
    void slotRequestPlayMusicItem(int row);
    void slotRequestCopyMusicItem(QList<int> rows);
    void slotRequestCutMusicItem(QList<int> rows);
    void slotRequestPasteMusicItem(int pos);
    void slotRequestExplorerMusicItem(int row);
    void slotRequestExportMusicItem(int row);
    void slotRequestViewMusicItem(int row);

    // Sort music list
    void slotRequestSortMusiclistAsName();
    void slotRequestSortMusiclistAsArtist();
    void slotRequestSortMusiclistAsAlbum();
    void slotRequestSortMusiclistAsDuration();
    void slotRequestSortMusiclistRandom();
    void slotRequestSortMusiclistReverse();

    // Track item
    void slotRequestPlayTrackItem(int row);
    void slotRequestCopyTrackItem(QList<int> rows);
    void slotRequestExportTrackItem(int row);
    void slotRequestViewTrackItem(int row);

    //// From controller
    void slotRequestUpdateModelsPlayingIndex(int pi, int mi, int ti);
    void slotRequestCurrentIndex(int *pIndex, int *mIndex, int *tIndex);
    void slotRequestFixDuration(int microSeconds);

    // Misc
    void slotAddExportMission();
    void slotRequestNextMusicProperty();
    void slotRequestPreviousMusicProperty();
    void slotRequestAdjustColumnWidth();// Adjust column width while duration is edited;

protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;

private:
    TExportMissions mExportMissions;
    TPlaylistModel *mPlaylistModel;
    TMusiclistModel *mMusiclistModel;
    TTrackListModel *mTracklistModel;
    TMissionsModel *mMissionsModel;

    TPlaylistWindow *mPlaylistWindow;
    TPlaylistWidget *mPlaylistWidget;
    TPlaylistView *mPlaylistView;
    TMusiclistView *mMusiclistView;
    TTracklistView *mTracklistView;

    TExportDialog *mExportDialog;
    TExportMissionsDialog *mExportMissionDialog;
    TPropertyDialog *mPropertyDialog;

    int mCurrentViewRow;
    bool mCurrentViewMusic;
    QMutex mExportMissionsLock;
    void fillPropertyDialog();
    void restoreMusicListSelection(int oldIndex);
    void locateIndex(int pi, int mi, int ti);
};

#endif // TPLAYLISTCONTROLLER_H
