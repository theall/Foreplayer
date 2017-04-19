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

#include "abstractpluginmanager.h"

#include <dirent.h>

#include "../cplusutils.h"

vector<wstring> getPathList(wstring basePath)
{
    _WDIR *dir;
    _wdirent *ptr;
    vector<wstring> pathList;
    _wopendir(basePath.c_str());

    if ((dir=_wopendir(basePath.c_str())) == NULL)
        return pathList;

    if(basePath[basePath.size()-1] != L'/')
        basePath += L"/";

    while ((ptr=_wreaddir(dir)) != NULL)
    {
        if(_wcsicmp(ptr->d_name, L".")==0 || _wcsicmp(ptr->d_name, L"..")==0)    ///current dir OR parrent dir
            continue;
        else
            pathList.push_back(basePath + ptr->d_name);
    }
    _wclosedir(dir);
    return pathList;
}

wstring TAbstractPluginManager::mPluginDir = L"plugins";

TAbstractPluginManager::TAbstractPluginManager() :
    mDefaultPlugin(NULL)
{
}

TAbstractPluginManager::~TAbstractPluginManager()
{
    for(auto plugin : mPlugins)
    {
        TBackendPlugin *backendPlugin = (TBackendPlugin*)plugin;
        if(backendPlugin)
        {
            backendPlugin->closeTrack();
            delete backendPlugin;
        }
    }
    mPlugins.clear();
}

void TAbstractPluginManager::setPluginDir(wstring dir)
{
    mPluginDir = dir;
}

int TAbstractPluginManager::findPlugins(wstring path)
{
    wstring abPath = getCurrentDir();
    abPath += L"/" + mPluginDir + L"/" + path;
    vector<wstring> pathList = getPathList(abPath);
    for(wstring filePath : pathList)
    {
#ifdef _WIN32
        filePath += L"/plugin.dll";
#elif defined(_UNIX)
        filePath += L"/plugin.so";
#endif
        TBackendPlugin* plugin = loadPlugin(filePath);
        if(plugin)
        {
            mPlugins.push_back(plugin);
            if(lower(extractBaseName(filePath))==L"default")
            {
                mDefaultPlugin = plugin;
            }
        }
    }

    return mPlugins.size();
}
