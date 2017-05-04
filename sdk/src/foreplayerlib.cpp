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

#define K_TEMP  "temp"

static TCore *g_core = nullptr;
static TPlayerCore *g_player = nullptr;
static TPlaylistCore *g_playlist = nullptr;

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
        if(g_playlist)
            *(void**)param2 = (void*)g_playlist->playlistItem(*(int*)param1);
        break;
    case CMD_GET_PLAYLIST_NAME:
        if(g_playlist)
        {
            TPlaylistItem *playlistItem = g_playlist->playlistItem(*(int*)param1);
            *(wstring*)param2 = playlistItem?playlistItem->name():L"";
        }
        break;
    case CMD_SET_PLAYLIST_NAME:
        if(g_playlist)
        {
            g_playlist->rename(*(int*)param1, *(wstring*)param2);
        }
        break;
    case CMD_GET_PLAYLIST_ITEM_INDEX:
        if(g_playlist)
        {
            *(int*)param2 = g_playlist->indexOf((TPlaylistItem*)param1);
        }
        break;
    case CMD_INSERT_PLAYLIST:
        if(g_playlist)
        {
            *(int*)param3 = g_playlist->insert(*(wstring*)param1, *(int*)param2);
        }
        break;
    case CMD_MOVE_PLAYLISTS:
        if(g_playlist)
        {
            list<int> indexes = *(list<int>*)param1;
            int pos = *(int*)param2;
            int listSize = g_playlist->size();
            TPlaylistItems items;
            list<int> indexesMoved;
            if(indexes.size()<=0 || listSize<=0)
                return;

            if(pos < 0)
                pos = 0;
            if(pos > listSize)
                pos = listSize;

            indexes.sort();
            indexes.reverse();

            for (int i : indexes) {
                if(i<0 || i>=listSize)
                    continue;
                items.push_back(g_playlist->takeAt(i));
                if(i < pos)
                    pos--;
            }
            int i = 0;
            for (TPlaylistItem *item : items) {
                g_playlist->insert(pos, item);
                indexesMoved.push_back(pos+i++);
            }
            *(list<int>*)param3 = indexesMoved;
        }
        break;
    case CMD_REMOVE_PLAYLIST:
        if(g_playlist)
        {
            g_playlist->remove(*(int*)param1);
        }
        break;
    case CMD_RENAME_PLAYLIST:
        if(g_playlist)
        {
            g_playlist->rename(*(int*)param1, *(wstring*)param2);
        }
        break;
    case CMD_SORT_PLAYLISTS:
        if(g_playlist)
        {
            g_playlist->sort(*(SortMethod*)param1);
        }
        break;
    case CMD_SET_AUDIO_PARAMETER:
        if(g_player)
        {
            g_player->setAudioParameter(*(AudioParameter*)param1, *(float*)param2, *(int*)param3);
        }
        break;
    case CMD_GET_AUDIO_DATA:
        if(g_player)
        {
            g_player->getAudioData(*(AudioDataType*)param1, param2, param3);
        }
        break;
    case CMD_GET_MUSIC_ITEM_COUNT:
        *(int*)param2 = param1?((TPlaylistItem*)param1)->size():0;
        break;
    case CMD_GET_MUSIC_ITEM:
        *(void**)param3 = param1?(void*)((TPlaylistItem*)param1)->musicItem(*(int*)param2):nullptr;
        break;
    case CMD_GET_MUSIC_ITEM_INDEX:
        *(int*)param3 = param1?((TPlaylistItem*)param1)->indexOf((TMusicItem*)param2):-1;
        break;
    case CMD_MOVE_MUSIC_ITEMS:
        {
            TPlaylistItem *playlistItem = (TPlaylistItem*)param1;
            list<int> indexes = *(list<int>*)param2;
            int pos = *(int*)param3;
            int listSize = playlistItem->size();
            TMusicItems items;
            list<int> indexesMoved;
            if(!playlistItem || indexes.size()<=0 || listSize<=0)
                return;

            if(pos < 0)
                pos = 0;
            if(pos > listSize)
                pos = listSize;

            indexes.sort();
            indexes.reverse();

            for (int i : indexes) {
                if(i<0 || i>=listSize)
                    continue;
                items.push_back(playlistItem->takeAt(i));
                if(i < pos)
                    pos--;
            }
            int i = 0;
            for (TMusicItem *item : items) {
                playlistItem->insert(pos, item);
                indexesMoved.push_back(pos+i++);
            }
            *(list<int>*)param4 = indexesMoved;
        }
        break;
    case CMD_INSERT_MUSIC_ITEM:
        *(int*)param4 = param1?((TPlaylistItem*)param1)->insert(*(int*)param2, (TMusicItem*)param3):-1;
        break;
    case CMD_REMOVE_MUSIC_ITEM:
        *(bool*)param3 = param1?((TPlaylistItem*)param1)->remove(*(int*)param2):false;
        break;
    case CMD_GET_MUSIC_ITEM_DISPLAY_NAME:
        *(wstring*)param2 = param1?((TMusicItem*)param1)->displayName():L"";
        break;
    case CMD_SET_MUSIC_ITEM_DISPLAY_NAME:
        *(bool*)param3 = param1?((TMusicItem*)param1)->setDisplayName(*(wstring*)param2):false;
        break;
    case CMD_GET_MUSIC_ITEM_FILE_NAME:
        *(wstring*)param2 = param1?((TMusicItem*)param1)->fileName():L"";
        break;
    case CMD_GET_MUSIC_ITEM_ARTIST:
        *(wstring*)param2 = param1?((TMusicItem*)param1)->artist():L"";
        break;
    case CMD_GET_MUSIC_ITEM_ALBUM:
        *(wstring*)param2 = param1?((TMusicItem*)param1)->album():L"";
        break;
    case CMD_GET_MUSIC_ITEM_YEAR:
        *(wstring*)param2 = param1?((TMusicItem*)param1)->year():L"";
        break;
    case CMD_GET_MUSIC_ITEM_TYPE:
        *(wstring*)param2 = param1?((TMusicItem*)param1)->system():L"";
        break;
    case CMD_GET_MUSIC_ITEM_ADDTIONAL_INFO:
        *(wstring*)param2 = param1?((TMusicItem*)param1)->additionalInfo():L"";
        break;
    case CMD_GET_MUSIC_ITEM_DURATION:
        *(int*)param2 = param1?((TMusicItem*)param1)->duration():0;
        break;
    case CMD_GET_MUSIC_ITEM_SAMPLE_RATE:
        *(int*)param2 = param1?((TMusicItem*)param1)->sampleRate():0;
        break;
    case CMD_GET_MUSIC_ITEM_CHANNELS:
        *(int*)param2 = param1?((TMusicItem*)param1)->channels():0;
        break;
    case CMD_GET_TRACK_ITEM_COUNT:
        *(int*)param2 = param1?((TMusicItem*)param1)->size():0;
        break;
    case CMD_GET_TRACK_ITEM:
        *(void**)param3 = param1?(void*)((TMusicItem*)param1)->trackItem(*(int*)param2):nullptr;
        break;
    case CMD_GET_TRACK_ITEM_NAME:
        *(wstring*)param2 = param1?((TTrackItem*)param1)->displayName:L"";
        break;
    case CMD_SET_TRACK_ITEM_NAME:
        {
            wstring newName = *(wstring*)param3;
            TTrackItem *trackItem = (TTrackItem*)param2;
            if(trackItem && !newName.empty() && newName != trackItem->displayName)
            {
                trackItem->displayName = newName;
                ((TMusicItem*)param1)->setModified();
                *(bool*)param4 = true;
            }
            *(bool*)param4 = false;
        }
        break;
    case CMD_GET_TRACK_ITEM_DURATION:
        *(int*)param2 = param1?((TTrackItem*)param1)->duration:0;
        break;
    case CMD_SET_TRACK_ITEM_DURATION:
        {
            int newDuration = *(int*)param3;
            TTrackItem *trackItem = (TTrackItem*)param2;
            if(trackItem && newDuration>=0 && newDuration!=trackItem->duration)
            {
                int diff = newDuration - trackItem->duration;
                trackItem->duration = newDuration;
                TMusicItem *musicItem = (TMusicItem*)param1;
                if(musicItem)
                {
                    musicItem->setDuration(musicItem->duration()+diff);
                    musicItem->setModified();
                }
                *(bool*)param4 = true;
            }
            *(bool*)param4 = false;
        }
        break;
    case CMD_GET_TRACK_ITEM_TYPE:
        *(wstring*)param2 = param1?((TTrackItem*)param1)->system:L"";
        break;
    case CMD_GET_TRACK_ITEM_YEAR:
        *(wstring*)param2 = param1?((TTrackItem*)param1)->year:L"";
        break;
    case CMD_GET_TRACK_ITEM_ADDITIONAL_INFO:
        *(wstring*)param2 = param1?((TTrackItem*)param1)->additionalInfo:L"";
        break;
    case CMD_GET_TRACK_ITEM_ENABLED:
        *(bool*)param2 = param1?((TTrackItem*)param1)->enable:true;
        break;
    case CMD_GET_TRACK_ITEM_INDEX_NAME:
        {
            TTrackItem *trackItem = (TTrackItem*)param1;
            if(!trackItem)
                *(wstring*)param2 = L"";
            else
            {
                if(trackItem->indexName.empty())
                    *(wstring*)param2 = trackItem->index;
                else
                    *(wstring*)param2 = trackItem->indexName;
            }
        }
        break;
    case CMD_GET_TRACK_ITEM_SAMPLE_RATE:
        *(int*)param2 = param1?((TTrackItem*)param1)->sampleRate:0;
        break;
    case CMD_GET_TRACK_ITEM_CHANNEL:
        *(int*)param2 = param1?((TTrackItem*)param1)->channels:0;
        break;
    case CMD_PARSE_FILE:
        if(g_playlist)
        {
            *(void**)param2 = (void*)g_playlist->parse(*(wstring*)param1);
        }
        break;
    case CMD_CLOSE:
        if(g_core)
        {
            delete g_core;
            g_core = nullptr;
            g_player = nullptr;
            g_playlist = nullptr;
        }
        break;
    case CMD_PLAYER_PLAY_TRACK:
        if(g_player)
        {
            *(bool*)param2 = g_player->playTrack((TTrackItem*)param1);
        } else {
            *(bool*)param2 = false;
        }
        break;
    case CMD_PLAYER_PAUSE:
        if(g_player)
        {
            g_player->pause();
            *(bool*)param1 = true;
        } else {
            *(bool*)param1 = false;
        }
        break;
    case CMD_PLAYER_SEEK_POSITION:
        *(bool*)param2 = g_player?g_player->seek(*(int*)param1):false;
        break;
    case CMD_PLAYLIST_ITEM_CLEAR:
        ((TPlaylistItem*)param1)->clear();
        break;
    case CMD_PLAYER_STOP:
        if(g_player)
        {
            g_player->stop();
            *(bool*)param1 = true;
        } else {
            *(bool*)param1 = false;
        }
        break;
    case CMD_GET_CURRENT_TRACK_ITEM:
        *(void**)param1 = g_playlist?g_playlist->currentTrackItem():NULL;
        break;
    case CMD_GET_TRACK_ITEM_ARTIST:
        *(wstring*)param2 = param1?((TTrackItem*)param1)->artist:L"";
        break;
    case CMD_PLAYER_IS_PLAYING:
        if(g_player)
            *(bool*)param1 = g_player->isPlaying();
        else
            *(bool*)param1 = false;
        break;
    case CMD_GET_TRACK_ITEM_ALBUM:
        *(wstring*)param2 = param1?((TTrackItem*)param1)->system:L"";
        break;
    case CMD_PLAYER_PLAY_INDEX:
        *(bool*)param4 = false;
        if(g_player && g_playlist)
        {
            if(TPlaylistItem *playlistItem = g_playlist->playlistItem(*(int*)param1))
                if(TMusicItem *musicItem = playlistItem->musicItem(*(int*)param2))
                    if(TTrackItem *trackItem = musicItem->trackItem(*(int*)param3))
                    {
                        *(bool*)param4 = g_player->playTrack(trackItem);
                        if(*(bool*)param4)
                            g_playlist->setPlayingIndex(*(int*)param1, *(int*)param2, *(int*)param3);
                    }
        }
        break;
    case CMD_GET_TRACK_ITEM_INDEX:
        *(int*)param3 = param1?((TMusicItem*)param1)->indexOf((TTrackItem*)param2):-1;
        break;
    case CMD_GET_PLAYED_TIME:
        *(int*)param1 = g_player?g_player->playedTime():-1;
        break;
    case CMD_PLAYER_IS_STOPED:
        *(bool*)param1 = g_player?g_player->isStoped():true;
        break;
    case CMD_PLAYLIST_REMOVE_REDUNDANT:
        *(list<int>*)param2 = param1?((TPlaylistItem*)param1)->removeRedundant():list<int>();
        break;
    case CMD_GET_CURRENT_PLAYLIST_ITEM:
        *(void**)param1 = g_playlist?g_playlist->currentPlaylistItem():NULL;
        break;
    case CMD_PLAYLIST_ITEM_SORT:
        if(param1)
            ((TPlaylistItem*)param1)->sort(*(SortMethod*)param2);
        break;
    case CMD_PLAYLIST_UPDATE_MUSIC_ITEM:
        if(param1)
            ((TPlaylistItem*)param1)->update(*(int*)param2, (TMusicItem*)param3);
        break;
    case CMD_PLAYER_IS_PAUSED:
        *(bool*)param1 = g_player?g_player->isPaused():false;
        break;
    case CMD_GET_TRACK_ITEMS:
        if(param1)
        {
            list<void*> trackItemsVoid;
            TTrackItems *trackItems = ((TMusicItem*)param1)->trackItems();
            for(TTrackItem *item : *trackItems)
            {
                trackItemsVoid.push_back(item);
            }
            *(list<void*>*)param2 = trackItemsVoid;
        }
        break;
    case CMD_GET_CURRENT_MUSIC_ITEM:
        *(void**)param1 = g_playlist?g_playlist->currentMusicItem():NULL;
        break;
    case CMD_GET_PLAYING_INDEX:
        if(g_playlist)
        {
            g_playlist->playingIndex((int*)param1, (int*)param2, (int*)param3);
        } else {
            *(int*)param1 = -1;
            *(int*)param2 = -1;
            *(int*)param3 = -1;
        }
        break;
    case CMD_SET_PLAYING_INDEX:
        if(g_playlist)
        {
            g_playlist->setPlayingIndex(*(int*)param1, *(int*)param2, *(int*)param3);
        }
    case CMD_PLAYER_RESUME:
        *(bool*)param1 = g_player?g_player->resume():false;
        break;
    case CMD_PLAYLIST_REMOVE_ERRORS:
        *(list<int>*)param2 = param1?((TPlaylistItem*)param1)->removeErrors():list<int>();
        break;
    case CMD_GET_PLUGIN_LIST:
        {
            TBackendPlugins plugins = TBackendPluginManager::instance()->plugins();
            list<void*> plguinHandles;
            for(TBackendPlugin *plugin : plugins)
                plguinHandles.push_back(plugin);

            *(list<void*>*)param1 = plguinHandles;
        }
        break;
    case CMD_GET_PLUGIN_NAME:
        *(wstring*)param2 = param1?((TBackendPlugin*)param1)->pluginInfo()->name:L"";
        break;
    case CMD_GET_PLUGIN_MANUFACTURE:
        *(wstring*)param2 = param1?((TBackendPlugin*)param1)->pluginInfo()->manufacture:L"";
        break;
    case CMD_GET_PLUGIN_CONTACT:
        *(wstring*)param2 = param1?((TBackendPlugin*)param1)->pluginInfo()->contact:L"";
        break;
    case CMD_GET_PLUGIN_CREATE_DATE:
        *(wstring*)param2 = param1?((TBackendPlugin*)param1)->pluginInfo()->createDate:L"";
        break;
    case CMD_GET_PLUGIN_DESCRIPTION:
        *(wstring*)param2 = param1?((TBackendPlugin*)param1)->pluginInfo()->description:L"";
        break;
    case CMD_GET_PLUGIN_SUFFIXDESCRIPTION:
        *(map<wstring, wstring>*)param2 = param1?((TBackendPlugin*)param1)->suffixListDescription():map<wstring, wstring>();
        break;
    case CMD_TRACK_ITEMS_AS_STRING:
        break;
    case CMD_STRING_TO_TRACK_ITEMS:
        break;
    case CMD_MUSIC_ITEMS_AS_STRING:
        if(param1 && param2)
        {
            json object;
            for(void *m : *(list<void*>*)param1)
            {
                TMusicItem *musicItem = (TMusicItem*)m;
                if(!musicItem)
                    continue;

                object.push_back(musicItem->toJson());
            }
            stringstream ss;
            ss << object;
            ss >> *(string*)param2;
        }
        break;
    case CMD_STRING_TO_MUSIC_ITEMS:
        if(param1 && param2)
        {
            stringstream ss;
            ss << *(string*)param1;
            json object;
            ss >> object;
            list<void*> musicItems;
            for (json o : object) {
                TMusicItem *musicItem = new TMusicItem;
                musicItem->fromJson(o);
                musicItems.push_back(musicItem);
            }
            *(list<void*>*)param2 = musicItems;
        }
        break;
    case CMD_EXPORT_FRAME_SAMPLE_COUNT:
        if(g_player)
        {
            *(int*)param3 = g_player->samplesPerFrame(*(int*)param1, *(int*)param2);
        } else {
            *(int*)param3 = 0;
        }
        break;
    case CMD_EXPORT_LOAD_TRACK:
        if(g_player)
        {
            *(bool*)param2 = g_player->loadTrack((TTrackItem*)param1);
        } else {
            *(bool*)param2 = false;
        }
        break;
    case CMD_EXPORT_NEXT_FRAME:
        if(g_player)
        {
            g_player->nextSamples((byte*)param1, *(int*)param2);
        } else {
            *(int*)param2 = 0;
        }
        break;
    default:
        break;
    }
}

