/*
 * Copyright (C) Kreogist Dev Team
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

TBackendPluginManager *TBackendPluginManager::mInstance=NULL;

//-------------------------------------------------------------------------------
TBackendPluginManager::TBackendPluginManager() :
    TAbstractPluginManager()
{
    //Check and build type list.
    findPlugins("backend");
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

TBackendPlugin *TBackendPluginManager::parse(QString file, TMusicInfo *musicInfo)
{
    TBackendPlugins backendPlugins;
    QFileInfo fi(file);
    QString suffix = fi.suffix();
    if(!suffix.isEmpty())
    {
        for(auto plugin : mPlugins)
        {
            TBackendPlugin *backendPlugin = (TBackendPlugin*)plugin;
            if(backendPlugin && backendPlugin->matchSuffix(suffix))
                backendPlugins.append(backendPlugin);
        }
        if(backendPlugins.isEmpty())
            backendPlugins = mPlugins;
    } else {
        if(fi.isDir())
        {
            // Enumerate files in directory

        } else {
            backendPlugins = mPlugins;
        }
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
    QFileInfo fi(trackInfo->musicFileName.c_str());
    QString suffix = fi.suffix();
    if(fi.isDir())
    {
        // Re-set suffix to index file if the track is a file under directory
        QDir dir = fi.absoluteDir();
        fi.setFile(dir.filePath(trackInfo->indexName.c_str()));
        suffix = fi.suffix();
    }
    if(!suffix.isEmpty())
    {
        for(auto plugin : mPlugins)
        {
            TBackendPlugin *backendPlugin = (TBackendPlugin*)plugin;
            if(backendPlugin && backendPlugin->matchSuffix(suffix))
                backendPlugins.append(backendPlugin);
        }
        if(backendPlugins.isEmpty())
            backendPlugins = mPlugins;
    }
    TBackendPlugin *backendPlugin = NULL;
    for(auto plugin : backendPlugins)
    {
        backendPlugin = dynamic_cast<TBackendPlugin*>(plugin);
        if(backendPlugin && backendPlugin->openTrack(trackInfo))
            break;
    }
    return backendPlugin;
}

TBackendPlugin* TBackendPluginManager::loadPlugin(QString pluginName)
{
    TBackendPlugin *plugin = new TBackendPlugin;
    if(!plugin->load(pluginName)) {
        delete plugin;
        plugin = NULL;
    }
    return plugin;
}
