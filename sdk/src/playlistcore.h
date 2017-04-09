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

typedef vector<int> intn;

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
    TPlaylistItem *takeAt(int plIndex);
    void insert(int pos, TPlaylistItem *item);
    void insert(wstring name, int index = -1);
    bool remove(int index);
    void rename(int index, wstring newName);
    void sort(SortMethod mode = SM_TITLE_ASC);
    int indexOf(TPlaylistItem *item);

    int playingPlaylistIndex();
    int playingMusicIndex();
    int playingTrackIndex();
    void playingIndex(int *pIndex, int *mIndex, int *tIndex);
    void setPlayingIndex(int pIndex, int mIndex, int tIndex);

    void exportAs(int index, wstring fileName);
    void exportAll(wstring path);

    /**
     * Music list functions.
     */
    TMusicItem *currentMusicItem();

    /**
     * Track list functions.
     */
    TTrackItem *currentTrackItem();

    /**
     * Parse track list from music.
     */
    TMusicItem *parse(wstring file);

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
    TBackendPluginManager *mBackendPluginManager;

    void findPlaylist();
    void loadPlaylist(wstring fileName);
    wstring getFileName();
};

#endif // TPLAYLISTCORE_H
