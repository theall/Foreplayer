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

#define DELETE_POINTER(x) \
    if(x)\
    {\
        delete x;\
        x = NULL;\
    }

TCore::TCore(bool exportMode):
    mPlayer(NULL)
  , mPlaylist(NULL)
  , mExporter(NULL)
  , mPlugins(TBackendPluginManager::instance())
{
    if(exportMode)
    {
        mExporter = new TExportCore();
    } else {
        mPlayer = new TPlayerCore();
        mPlaylist = new TPlaylistCore();
    }
}

TCore::~TCore()
{
    DELETE_POINTER(mPlayer);
    DELETE_POINTER(mPlaylist);
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

TExportCore *TCore::exporter()
{
    return mExporter;
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
