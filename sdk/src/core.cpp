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
#include "core.h"

TCore::TCore()
{
    mPlayer = new TPlayerCore();
    mPlaylist = new TPlaylistCore();
    mPlugins = TBackendPluginManager::instance();
}

TCore::~TCore()
{
    if(mPlayer)
    {
        delete mPlayer;
        mPlayer = NULL;
    }
    if(mPlaylist)
    {
        delete mPlaylist;
        mPlaylist = NULL;
    }
    if(mPlugins)
    {
        TBackendPluginManager::deleteInstance();
        mPlugins = NULL;
    }
}

TPlayerCore *TCore::player()
{
    return mPlayer;
}

TPlaylistCore *TCore::playlist()
{
    return mPlaylist;
}

vector<wstring> TCore::playlists()
{
    return mPlaylist->names();
}

void TCore::open()
{

}

void TCore::close()
{

}
