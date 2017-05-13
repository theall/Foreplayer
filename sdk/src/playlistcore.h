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
#ifndef TPLAYLISTCORE_H
#define TPLAYLISTCORE_H

#include "playlist/playlistitem.h"
#include "pluginmanager/backendpluginmanager.h"

#include <mutex>

typedef list<int> intn;

class TPlaylistCore
{
public:
    explicit TPlaylistCore();
    ~TPlaylistCore();

    static void setPlaylistDir(wstring dir);

    /**
     * Playlist functions
     */
    vector<wstring> names();
    int size();
    TPlaylistItem *currentPlaylistItem();
    TPlaylistItem *playlistItem(int plIndex = -1);
    list<int> move(list<int> indexes, int pos);
    int insert(wstring name, int index = -1);
    bool remove(int index);
    void rename(int index, wstring newName);
    void sort(SortMethod mode = SM_TITLE_ASC);
    int indexOf(TPlaylistItem *item);

    int playingPlaylistIndex();
    void setPlayingPlaylistIndex(int index);
    int playingMusicIndex();
    void setPlayingMusicListIndex(int index);
    int playingTrackIndex();
    void setPlayingTrackListIndex(int index);
    void playingIndex(int *pIndex, int *mIndex, int *tIndex);
    void setPlayingIndex(int pIndex, int mIndex, int tIndex);

    void exportAs(int index, wstring fileName);
    void exportAll(wstring path);

    /**
     * Music list functions.
     */
    TMusicItem *currentMusicItem();
    list<int> removeRedundant(TPlaylistItem *playlistItem);
    list<int> removeErrors(TPlaylistItem *playlistItem);
    list<int> removeMusicItems(TPlaylistItem *playlistItem, list<int> indexes);
    list<int> moveMusicItems(TPlaylistItem *playlistItem, list<int> indexes, int pos);
    list<int> insertMusicItems(TPlaylistItem *playlistItem, int pos, TMusicItems musicItems);
    void removeAllMusicItems(TPlaylistItem *playlistItem);
    void sort(TPlaylistItem *playlistItem, SortMethod sm);

    /**
     * Track list functions.
     */
    TTrackItem *currentTrackItem();

    /**
     * Parse track list from music.
     */
    static TMusicItem *parse(wstring file);

    void save();

private:
    static wstring mPluginDir;
    TPlaylistItems mPlaylist;

    // Current playing index
    int mPlaylistIndex;
    int mMusiclistIndex;
    int mTracklistIndex;
    wstring mCurrentDir;
    wstring mPlaylistDir;
    bool mFileSaving;

    mutex mMutex;

    void findPlaylist();
    void loadPlaylist(wstring fileName);
    wstring getFileName();
    TPlaylistItem *takeAt(int plIndex);
    int insert(int pos, TPlaylistItem *item);
};

#endif // TPLAYLISTCORE_H
