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
#ifndef TCORE_H
#define TCORE_H

#include <foreplayer.h>

#include <QLibrary>

typedef QList<int> intn;
typedef void* PlayListItem;
typedef void* MusicItem;
typedef void* TrackItem;
typedef QList<PlayListItem> TPlayListItems;
typedef QList<MusicItem> MusicItems;
typedef QList<TrackItem> TrackItems;

enum IndexType
{
    IT_PL,
    IT_ML,
    IT_TL
};

#define MIME_TYPE_PLAY_LIST     "Foreplayer/Playlist"
#define MIME_TYPE_MUSIC_ITEM    "Foreplayer/MusicItem"
#define MIME_TYPE_TRACK_ITEM    "Foreplayer/TrackItem"

class TCore
{
public:
    TCore();
    ~TCore();

    static TCore *instance();
    static void deleteInstance();

    // Return playlist count
    int playlistCount();
    QStringList playlists();
    int insertPlaylist(QString name, int pos = -1);
    void removePlaylist(int index);
    QList<int> movePlaylists(QList<int> indexes, int pos);
    void renamePlaylist(int index, QString name);
    void sort(SortMethod sm = SM_TITLE_ASC);

    // Return playlist item
    PlayListItem getPlaylistItem(int index);
    int getPlaylistItemIndex(PlayListItem item);
    QString getPlaylistName(int index);
    void setPlaylistName(int index, QString newName);

    int insertMusicItem(PlayListItem playlistItem, int pos, MusicItem musicItem);
    bool removeMusicItem(PlayListItem playlistItem, int pos);
    bool updateMusicItem(PlayListItem playlistItem, int pos, MusicItem musicItem);

    QList<int> removeRedundant(PlayListItem playlistItem);
    QList<int> removeErrors(PlayListItem playlistItem);
    void clear(PlayListItem playlistItem);
    void sort(PlayListItem playlistItem, SortMethod sm);

    //// Music items
    int getMusicItemCount(PlayListItem playlistItem);
    MusicItem getMusicItem(PlayListItem playlistItem, int row);
    QString musicItemToString(MusicItem musicItem);
    QString musicItemsToString(MusicItems musicItems);
    MusicItem stringToMusicItem(QString str);
    MusicItems stringToMusicItems(QString str);
    int getMusicItemIndex(PlayListItem playlistItem, MusicItem musicItem);
    QList<int> moveMusicItems(PlayListItem playlistItem, QList<int> indexes, int pos);

    QString getMusicItemDisplayName(MusicItem musicItem);
    QString getMusicItemFileName(MusicItem musicItem);
    QString getMusicItemArtist(MusicItem musicItem);
    QString getMusicItemAlbum(MusicItem musicItem);
    QString getMusicItemType(MusicItem musicItem);
    QString getMusicItemYear(MusicItem musicItem);
    QString getMusicItemAdditionalInfo(MusicItem musicItem);
    int getMusicItemDuration(MusicItem musicItem);

    bool setMusicItemName(MusicItem musicItem, QString newName);

    //// Track items
    int getTrackItemCount(MusicItem musicItem);
    TrackItem getTrackItem(MusicItem musicItem, int index);
    QString trackItemToString(TrackItem trackItem);
    TrackItems getTrackItems(MusicItem musicItem);
    QString trackItemsToString(TrackItems trackItems);
    int getTrackItemIndex(MusicItem musicItem, TrackItem trackItem);
    QString getTrackItemName(TrackItem trackItem);
    bool setTrackItemName(MusicItem musicItem, TrackItem trackItem, QString newName);
    bool setTrackItemDuration(MusicItem musicItem, TrackItem trackItem, int duration);

    int getTrackItemDuration(TrackItem trackItem);
    QString getTrackItemType(TrackItem trackItem);
    QString getTrackItemArtist(TrackItem trackItem);
    QString getTrackItemAlbum(TrackItem trackItem);
    QString getTrackItemYear(TrackItem trackItem);
    QString getTrackItemAdditionalInfo(TrackItem trackItem);
    QString getTrackItemIndexName(TrackItem trackItem);
    bool isTrackItemEnabled(TrackItem trackItem);

    // Parse file
    MusicItem parse(QString filePath);

    //// Player
    bool playTrackItem(TrackItem trackItem);
    bool playIndex(int pi, int mi, int ti);
    void getPlayingIndex(int *pi, int *mi, int *ti);
    void setPlayingIndex(int pi, int mi, int ti);
    int getPlayingIndex(IndexType it);
    bool stop();
    bool resume();
    bool isPaused();
    bool isStoped();
    bool isPlaying();
    bool pause();

    void setAudioParameter(AudioParameter type, float value, int param = 0);
    void getAudioData(AudioDataType dataType, void *param1, void *param2);
    PlayListItem getPlayingPlaylistItem();
    MusicItem getPlayingMusicItem();
    TrackItem getPlayingTrackItem();
    int getCurrentPlayedTime();

private:
    QLibrary *mLibrary;
    FOREPLAYER_SEND_CMD mSendCmd;

    static TCore *mInstance;
};

#endif // TCORE_H
