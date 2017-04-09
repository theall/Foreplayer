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

#include <foreplayer.h>

#include "core.h"

static TCore *g_core = NULL;
static TPlayerCore *g_player = NULL;
static TPlaylistCore *g_playlist = NULL;

void FOREPLAYER_API foreplayer_send_cmd(
        int cmd,
        void *param1,
        void *param2,
        void *param3,
        void *param4)
{
    ForeplayerCmd fcmd = (ForeplayerCmd)cmd;
    switch(fcmd)
    {
        case CMD_OPEN:
            if(!g_core)
            {
                g_core = new TCore;
                g_player = g_core->player();
                g_playlist = g_core->playlist();
            }
            break;
        case CMD_SETPLAYLISTPATH:
            TPlaylistCore::setPlaylistDir(*(wstring*)param1);
            break;
        case CMD_SETPLUGINSPATH:
            TAbstractPluginManager::setPluginDir(*(wstring*)param1);
            break;
        case CMD_GET_PLAYLISTS_NAME_LIST:
            if(g_core)
                *(vector<wstring>*)param1 = g_core->playlists();
            break;
        case CMD_GET_PLAYLIST_ITEM:
            break;
        case CMD_GET_PLAYLIST_NAME:
            break;
        case CMD_SET_PLAYLIST_NAME:
            break;
        case CMD_GET_PLAYLIST_ITEM_INDEX:
            break;
        case CMD_INSERT_PLAYLIST:
            break;
        case CMD_MOVE_PLAYLISTS:
            break;
        case CMD_REMOVE_PLAYLIST:
            break;
        case CMD_RENAME_PLAYLIST:
            break;
        case CMD_SORT_PLAYLISTS:
            break;
        case CMD_SET_AUDIO_PARAMETER:
            break;
        case CMD_GET_MUSIC_ITEM_COUNT:
            break;
        case CMD_GET_MUSIC_ITEM:
            break;
        case CMD_GET_MUSIC_ITEM_INDEX:
            break;
        case CMD_MOVE_MUSIC_ITEMS:
            break;
        case CMD_INSERT_MUSIC_ITEM:
            break;
        case CMD_REMOVE_MUSIC_ITEM:
            break;
        case CMD_GET_MUSIC_ITEM_DISPLAY_NAME:
            break;
        case CMD_SET_MUSIC_ITEM_DISPLAY_NAME:
            break;
        case CMD_GET_MUSIC_ITEM_FILE_NAME:
            break;
        case CMD_GET_MUSIC_ITEM_ARTIST:
            break;
        case CMD_GET_MUSIC_ITEM_ALBUM:
            break;
        case CMD_GET_MUSIC_ITEM_YEAR:
            break;
        case CMD_GET_MUSIC_ITEM_TYPE:
            break;
        case CMD_GET_MUSIC_ITEM_ADDTIONAL_INFO:
            break;
        case CMD_GET_MUSIC_ITEM_DURATION:
            break;
        case CMD_GET_TRACK_ITEM_COUNT:
            break;
        case CMD_GET_TRACK_ITEM:
            break;
        case CMD_GET_TRACK_ITEM_NAME:
            break;
        case CMD_SET_TRACK_ITEM_NAME:
            break;
        case CMD_GET_TRACK_ITEM_DURATION:
            break;
        case CMD_SET_TRACK_ITEM_DURATION:
            break;
        case CMD_GET_TRACK_ITEM_TYPE:
            break;
        case CMD_GET_TRACK_ITEM_YEAR:
            break;
        case CMD_GET_TRACK_ITEM_ADDITIONAL_INFO:
            break;
        case CMD_GET_TRACK_ITEM_ENABLED:
            break;
        case CMD_GET_TRACK_ITEM_INDEX_NAME:
            break;
        case CMD_PARSE_FILE:
            break;
        case CMD_CLOSE:
            break;
    }
}

