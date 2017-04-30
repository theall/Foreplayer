/*
 * Foreplayer sdk header file.
 * Copyright 2015, Bilge Theall <wazcd_1608@qq.com>
 *
 * This file is part of Foreplayer.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FOREPLAYER_H
#define FOREPLAYER_H

#include <string>
#include <list>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  FOREPLAYER_EXPORT
#define FOREPLAYER_API __declspec(dllexport)
#else
#define FOREPLAYER_API __declspec(dllimport)
#endif

#ifdef _WIN32
    #define FOREPLAYER_LIB_NAME "foreplayerlib.dll"
#else
    #define FOREPLAYER_LIB_NAME "libforeplayerlib.so"
#endif

enum AudioParameter
{
    AP_VOLUME,
    AP_VOLUME_ENABLE,
    AP_BALLANCE,
    AP_EFFECT,
    AP_AMPLIFICATION,
    AP_EQUALIZER_ENABLE,
    AP_EQUALIZER_FACTOR,
    AP_EQUALIZER_RANGE,
    AP_RESET
};

enum AudioDataType
{
    ADT_SAMPLE,
    ADT_SPECTRUM,
    ADT_SILENT_FRAME,
    ADT_SILENT_MICRO_SECONDS
};

struct TSpectrumElement {
    TSpectrumElement()
    :   frequency(0.0), amplitude(0.0), phase(0.0), clipped(false)
    { }

    /**
     * Frequency in Hertz
     */
    float frequency;

    /**
     * Amplitude in range [0.0, 1.0]
     */
    float amplitude;

    /**
     * Phase in range [0.0, 2*PI]
     */
    float phase;

    /**
     * Indicates whether value has been clipped during spectrum analysis
     */
    bool clipped;
};

enum SortMethod
{
    SM_TITLE_ASC,
    SM_TITLE_DES,
    SM_LENGTH_ASC,
    SM_LENGTH_DES,
    SM_DURATION_ASC,
    SM_DURATION_DES,
    SM_INDEX_ASC,
    SM_INDEX_DES,
    SM_FILE_ASC,
    SM_FILE_DES,
    SM_ARTIST_ASC,
    SM_ARTIST_DES,
    SM_ALBUM_ASC,
    SM_ALBUM_DES,
    SM_GAME_ASC,
    SM_GAME_DES,
    SM_SYSTEM_ASC,
    SM_SYSTEM_DES,
    SM_YEAR_ASC,
    SM_YEAR_DES,
    SM_RANDOM,
    SM_REVERSE
};

enum ForeplayerCmd
{
    /**
     * Open sdk, this may take a few seconds to load all plugins and playlists
     * param1: return a bool value
     */
    CMD_OPEN,

    /**
     * Set playlist's store path, default in $approot/playlist, param1 pointer to wstring path.
     * Param1: pointer to wstring of path
     */
    CMD_SETPLAYLISTPATH,

    /**
     * Set plugins's store path, default in $approot/plugins, param1 pointer to wstring path.
     * Param1: pointer to wstring of path
     */
    CMD_SETPLUGINSPATH,

    /**
     * Get playlists.
     * param1: return pointer to std::wstring
     */
    CMD_GET_PLAYLISTS_NAME_LIST,

    /**
     * Get playlist item.
     * param1: int pointer to playlist item index
     * param2: return internal pointer to playlist item
     */
    CMD_GET_PLAYLIST_ITEM,

    /**
     * Get playlist name.
     * param1: int pointer to playlist item index
     * param2: return wstring of playlist item name
     */
    CMD_GET_PLAYLIST_NAME,

    /**
     * Set playlist name.
     * param1: int pointer to playlist item index
     * param2: pointer wstring of playlist item new name
     */
    CMD_SET_PLAYLIST_NAME,

    /**
     * Get playlist item index.
     * param1: playlist item
     * param2: return int pointer of playlist item index
     */
    CMD_GET_PLAYLIST_ITEM_INDEX,

    /**
     * Insert new playlist
     * param1: playlist name pointer to wstring
     * param2: return index of new playlist inserted
     */
    CMD_INSERT_PLAYLIST,

    /**
     * Move playlist items
     * param1: index list of items to move
     * param2: position move to
     * param3: new index list of moved items
     */
    CMD_MOVE_PLAYLISTS,

    /**
     * Remove playlist
     * param1: int pointer to playlist index
     */
    CMD_REMOVE_PLAYLIST,

    /**
     * Rename playlist
     * param1: int pointer to playlist index
     */
    CMD_RENAME_PLAYLIST,

    /**
     * Sort playlists
     */
    CMD_SORT_PLAYLISTS,

    /**
     * Change audio parameters.
     * param1: value of AudioParameter
     * param2:
     * param3:
     */
    CMD_SET_AUDIO_PARAMETER,

    /**
     * Get audio data.
     * param1: value of AudioDataType
     * param2: additional parameter 1
     * param3: additional parameter 2
     * param4: unused
     */
    CMD_GET_AUDIO_DATA,

    /**
     * Get current playing playlist item.
     * param1: return pointer of void *
     */
    CMD_GET_CURRENT_PLAYLIST_ITEM,

    /**
     * Get current playing music item.
     * param1: return pointer of void *
     */
    CMD_GET_CURRENT_MUSIC_ITEM,

    /**
     * Get current playing track item.
     * param1: return pointer of void *
     */
    CMD_GET_CURRENT_TRACK_ITEM,

    /**
     * Get current played time.
     * param1: return microseconds
     */
    CMD_GET_PLAYED_TIME,

    /**
     * Query player status.
     * param1: return bool
     */
    CMD_PLAYER_IS_PAUSED,

    /**
     * Play track item.
     * param1: track item handle
     * param2: return bool
     */
    CMD_PLAYER_PLAY_TRACK,

    /**
     * Update music item in playlist item
     * param1: playlist item handle
     * param2: position
     * param3: new music item handle
     * param4: return bool
     */
    CMD_PLAYLIST_UPDATE_MUSIC_ITEM,

    /**
     * Clear all music items in playlist.
     * param1: playlist handle
     */
    CMD_PLAYLIST_ITEM_CLEAR,

    /**
     * Get playing index.
     * param1: return int of playlist index
     * param2: return int of music item index
     * param3: return int of track item index
     */
    CMD_GET_PLAYING_INDEX,

    /**
     * Set playing index.
     * param1: int pointer of playlist index
     * param2: int pointer of music item index
     * param3: int pointer of track item index
     */
    CMD_SET_PLAYING_INDEX,

    /**
     * Remove redundant music items in specify playlist item.
     * param1: playlist item handle
     * param2: return list<int> indexes removed
     */
    CMD_PLAYLIST_REMOVE_REDUNDANT,

    /**
     * Query player status.
     * param1: return bool
     */
    CMD_PLAYER_IS_PLAYING,

    /**
     * Query player status.
     * param1: return bool
     */
    CMD_PLAYER_IS_STOPED,

    /**
     * Resume playing if player is paused.
     * param1: return bool
     */
    CMD_PLAYER_RESUME,

    /**
     * Sort music items in playlist.
     * param1: playlist item handle
     * param2: pointer to SortMethod
     */
    CMD_PLAYLIST_ITEM_SORT,

    /**
     * Pause player.
     * param1: return bool
     */
    CMD_PLAYER_PAUSE,

    /**
     * Seek position.
     * param1: micro seconds
     * param2: return bool
     */
    CMD_PLAYER_SEEK_POSITION,

    /**
     * Stop player.
     * param1: return bool
     */
    CMD_PLAYER_STOP,

    /**
     * Play track item specified by index.
     * param1: int of playlist index
     * param2: int of music item index
     * param3: int of track item index
     * param4: return bool
     */
    CMD_PLAYER_PLAY_INDEX,

    /**
     * Remove error music items in playlist.
     * param1: playlist handle
     * param2: return list<int> indexes removed
     */
    CMD_PLAYLIST_REMOVE_ERRORS,

    /**
     * Get music item count in playlist item.
     * param1: playlist item handle
     * param2: return a int value
     */
    CMD_GET_MUSIC_ITEM_COUNT,

    /**
     * Get music item in playlist item.
     * param1: playlist item handle
     * param2: music item index in playlist item
     * param3: return pointer to music item
     */
    CMD_GET_MUSIC_ITEM,

    /**
     * Convert music item to wstring.
     * param1: music item handle
     * param2: return wstring
     */
    CMD_MUSIC_ITEM_AS_STRING,

    /**
     * Convert music items to wstring.
     * param1: music item handle list
     * param2: return wstring
     */
    CMD_MUSIC_ITEMS_AS_STRING,

    /**
     * Convert wstring to music item.
     * param1: wstring
     * param2: return music item handle
     */
    CMD_STRING_TO_MUSIC_ITEM,

    /**
     * Convert wstring to music item array.
     * param1: wstring
     * param2: return array of music item handle
     */
    CMD_STRING_TO_MUSIC_ITEMS,

    /**
     * Get music item index in playlist item.
     * param1: playlist item handle
     * param2: return int
     */
    CMD_GET_MUSIC_ITEM_INDEX,

    /**
     * Move music items
     * param1: index list of items to move
     * param2: position move to
     * param3: new index list of moved items
     */
    CMD_MOVE_MUSIC_ITEMS,

    /**
     * Insert music item into playlist item
     * param1: playlist item handle
     * param2: position
     * param3: music item handle
     * param4: return position inserted
     */
    CMD_INSERT_MUSIC_ITEM,

    /**
     * Remove music item from playlist item
     * param1: playlist item handle
     * param2: position
     * param3: return bool
     */
    CMD_REMOVE_MUSIC_ITEM,

    /**
     * Get music item display name.
     * param1: music item handle
     * param2: return pointer to wstring
     */
    CMD_GET_MUSIC_ITEM_DISPLAY_NAME,

    /**
     * Set music item display name.
     * param1: music item handle
     * param2: wstring of track item new name
     * param3: return true if name changed
     */
    CMD_SET_MUSIC_ITEM_DISPLAY_NAME,

    /**
     * Get music item file full name.
     * param1: music item handle
     * param2: return pointer to wstring
     */
    CMD_GET_MUSIC_ITEM_FILE_NAME,

    /**
     * Get music item artist name.
     * param1: music item handle
     * param2: return pointer to wstring
     */
    CMD_GET_MUSIC_ITEM_ARTIST,

    /**
     * Get music item album name.
     * param1: music item handle
     * param2: return pointer to wstring
     */
    CMD_GET_MUSIC_ITEM_ALBUM,

    /**
     * Get music item's year.
     * param1: music item handle
     * param2: return pointer to int
     */
    CMD_GET_MUSIC_ITEM_YEAR,

    /**
     * Get music item type.
     * param1: music item handle
     * param2: return pointer to wstring
     */
    CMD_GET_MUSIC_ITEM_TYPE,

    /**
     * Get music item 's additional information.
     * param1: music item handle
     * param2: return pointer to wstring
     */
    CMD_GET_MUSIC_ITEM_ADDTIONAL_INFO,

    /**
     * Get music item duration.
     * param1: music item handle
     * param2: return pointer to int
     */
    CMD_GET_MUSIC_ITEM_DURATION,

    /**
     * Get music item sample rate.
     * param1: music item handle
     * param2: return pointer to int
     */
    CMD_GET_MUSIC_ITEM_SAMPLE_RATE,

    /**
     * Get music item channels.
     * param1: music item handle
     * param2: return pointer to int
     */
    CMD_GET_MUSIC_ITEM_CHANNELS,

    /**
     * Get track item count.
     * param1: music item handle
     * param2: return pointer to int
     */
    CMD_GET_TRACK_ITEM_COUNT,

    /**
     * Get track item.
     * param1: music item handle
     * param2: track item index
     * param3: return internal pointer to track item
     */
    CMD_GET_TRACK_ITEM,

    /**
     * Convert track item to wstring.
     * param1: track item handle
     * param2: return wstring
     */
    CMD_TRACK_ITEM_AS_STRING,

    /**
     * Convert wstring to track item.
     * param1: wstring
     * param2: return track item handle
     */
    CMD_STRING_TO_TRACK_ITEM,

    /**
     * Convert track items to wstring.
     * param1: list of track item handle
     * param2: return wstring
     */
    CMD_TRACK_ITEMS_AS_STRING,

    /**
     * Convert wstring to track item list.
     * param1: wstring
     * param2: return track item handle list
     */
    CMD_STRING_TO_TRACK_ITEMS,

    /**
     * Get track items.
     * param1: music item handle
     * param2: return list<trackitem handle>
     */
    CMD_GET_TRACK_ITEMS,

    /**
     * Get track item index in music item.
     * param1: music item handle
     * param2: track item handle
     * param3: return int
     */
    CMD_GET_TRACK_ITEM_INDEX,

    /**
     * Get track item name.
     * param1: track item handle
     * param2: return wstring of track item name
     */
    CMD_GET_TRACK_ITEM_NAME,

    /**
     * Set track item name.
     * param1: track item handle
     * param2: wstring of track item new name
     * param3: return true if name changed
     */
    CMD_SET_TRACK_ITEM_NAME,

    /**
     * Get track item duration.
     * param1: track item handle
     * param2: return int of micro seconds
     */
    CMD_GET_TRACK_ITEM_DURATION,

    /**
     * Set track item duration.
     * param1: track item handle
     * param2: int of new duration in micro seconds
     * param3: return true if duration changed
     */
    CMD_SET_TRACK_ITEM_DURATION,

    /**
     * Get track item type.
     * param1: track item handle
     * param2: return wstring
     */
    CMD_GET_TRACK_ITEM_TYPE,

    /**
     * Get track item artist.
     * param1: track item handle
     * param2: return wstring
     */
    CMD_GET_TRACK_ITEM_ARTIST,

    /**
     * Get track item album.
     * param1: track item handle
     * param2: return wstring
     */
    CMD_GET_TRACK_ITEM_ALBUM,

    /**
     * Get track item year.
     * param1: track item handle
     * param2: return int
     */
    CMD_GET_TRACK_ITEM_YEAR,

    /**
     * Get track item additional information.
     * param1: track item handle
     * param2: return wstring
     */
    CMD_GET_TRACK_ITEM_ADDITIONAL_INFO,

    /**
     * Retreive track item enable/disable.
     * param1: track item handle
     * param2: return bool
     */
    CMD_GET_TRACK_ITEM_ENABLED,

    /**
     * Retreive track item index name.
     * param1: track item handle
     * param2: return bool
     */
    CMD_GET_TRACK_ITEM_INDEX_NAME,

    /**
     * Retreive track item sample rate.
     * param1: track item handle
     * param2: return int
     */
    CMD_GET_TRACK_ITEM_SAMPLE_RATE,

    /**
     * Retreive track item channels.
     * param1: track item handle
     * param2: return int
     */
    CMD_GET_TRACK_ITEM_CHANNEL,

    /**
     * Parse file
     * param1: file full path
     * param2: return music item handle
     */
    CMD_PARSE_FILE,

    /**
     * Get plugin list.
     * param1: return std::list<plugin handle>
     */
    CMD_GET_PLUGIN_LIST,

    /**
     * Get plugin name.
     * param1: plugin handle
     * param2: return wstring
     */
    CMD_GET_PLUGIN_NAME,

    /**
     * Get plugin manufacture.
     * param1: plugin handle
     * param2: return wstring
     */
    CMD_GET_PLUGIN_MANUFACTURE,

    /**
     * Get plugin contact.
     * param1: plugin handle
     * param2: return wstring
     */
    CMD_GET_PLUGIN_CONTACT,

    /**
     * Get plugin create date.
     * param1: plugin handle
     * param2: return wstring
     */
    CMD_GET_PLUGIN_CREATE_DATE,

    /**
     * Get plugin description.
     * param1: plugin handle
     * param2: return wstring
     */
    CMD_GET_PLUGIN_DESCRIPTION,

    /**
     * Get plugin suffixdescription.
     * param1: plugin handle
     * param2: return map<wstring suffix, wstring description>
     */
    CMD_GET_PLUGIN_SUFFIXDESCRIPTION,

    /**
     * Load track item for customize.
     * param1: track item handle
     * param2: return bool
     */
    CMD_EXPORT_LOAD_TRACK,

    /**
     * Retrieve next frame data.
     * param1: pointer to buffer
     * param2: pointer to buffer size
     */
    CMD_EXPORT_NEXT_FRAME,

    /**
     * Retrueve sample count per frame.
     * param1: sample rate
     * param2: frames per second
     */
    CMD_EXPORT_FRAME_SAMPLE_COUNT,

    // Close sdk;
    CMD_CLOSE
};

void FOREPLAYER_API foreplayer_send_cmd(
        int cmd,
        void *param1,
        void *param2,
        void *param3,
        void *param4);

#define FOREPLAYER_SEND_CMD_NAME "foreplayer_send_cmd"

typedef void (*FOREPLAYER_SEND_CMD)(int cmd, void *param1, void *param2, void *param3, void *param4);

#ifdef __cplusplus
}
#endif

#endif // FOREPLAYER_H
