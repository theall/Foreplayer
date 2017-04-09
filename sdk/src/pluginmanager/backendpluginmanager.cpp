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

#include "backendpluginmanager.h"

#include "../cplusutils.h"

TBackendPluginManager *TBackendPluginManager::mInstance=NULL;

//-------------------------------------------------------------------------------
TBackendPluginManager::TBackendPluginManager() :
    TAbstractPluginManager()
{
    //Check and build type list.
    findPlugins(L"backend");
}

TBackendPluginManager::~TBackendPluginManager()
{

}

TBackendPluginManager *TBackendPluginManager::instance()
{
    if(!mInstance)
        mInstance = new TBackendPluginManager;

    return mInstance;
}

void TBackendPluginManager::deleteInstance()
{
    if(mInstance) {
        delete mInstance;
        mInstance = NULL;
    }
}

TBackendPlugin *TBackendPluginManager::parse(wstring file, TMusicInfo *musicInfo)
{
    TBackendPlugins backendPlugins;
    wstring suffix = extractSuffix(file);
    if(!suffix.empty())
    {
        for(auto plugin : mPlugins)
        {
            TBackendPlugin *backendPlugin = (TBackendPlugin*)plugin;
            if(backendPlugin && backendPlugin->matchSuffix(suffix))
                backendPlugins.push_back(backendPlugin);
        }
        if(backendPlugins.empty())
            backendPlugins.push_back(mDefaultPlugin);
    } else {
        backendPlugins.push_back(mDefaultPlugin);
    }
    TBackendPlugin *backendPlugin = NULL;
    for(auto plugin : backendPlugins)
    {
        backendPlugin = dynamic_cast<TBackendPlugin*>(plugin);
        if(backendPlugin && backendPlugin->parse(file, musicInfo))
            break;
    }
    return backendPlugin;
}

TBackendPlugin *TBackendPluginManager::loadTrack(TTrackInfo *trackInfo)
{
    TBackendPlugins backendPlugins;
    wstring suffix = extractSuffix(trackInfo->musicFileName);
    if(suffix.empty())
    {
        // Re-set suffix to index file if the track is a file under directory
        suffix = extractSuffix(trackInfo->indexName);
    }
    if(!suffix.empty())
    {
        for(auto plugin : mPlugins)
        {
            TBackendPlugin *backendPlugin = (TBackendPlugin*)plugin;
            if(backendPlugin && backendPlugin->matchSuffix(suffix))
                backendPlugins.push_back(backendPlugin);
        }
        if(backendPlugins.empty())
            backendPlugins = mPlugins;
    }
    TBackendPlugin *pluginFind = NULL;
    for(auto plugin : backendPlugins)
    {
        TBackendPlugin *backendPlugin = dynamic_cast<TBackendPlugin*>(plugin);
        if(backendPlugin && backendPlugin->openTrack(trackInfo))
        {
            pluginFind = backendPlugin;
            break;
        }
    }
    return pluginFind;
}

TBackendPlugin* TBackendPluginManager::loadPlugin(wstring pluginName)
{
    TBackendPlugin *plugin = new TBackendPlugin;
    wstring oldCurrentPath = getCurrentDir();
    setCurrentDirectory(extractPath(pluginName));
    if(!plugin->load(pluginName)) {
        delete plugin;
        plugin = NULL;
    }
    setCurrentDirectory(oldCurrentPath);
    return plugin;
}
