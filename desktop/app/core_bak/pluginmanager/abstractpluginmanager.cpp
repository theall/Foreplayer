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
#include <QCoreApplication>
#include <QDir>

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

int TAbstractPluginManager::findPlugins(QString path)
{
    QString abPath = qApp->applicationDirPath() + "/plugins/" + path;
    QDir dir(abPath);
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString filePath = fileInfo.filePath();
#ifdef Q_OS_WIN32
        filePath += "/plugin.dll";
#elif defined(Q_OS_UNIX)
        filePath += "/plugin.so";
#endif
        TBackendPlugin* plugin = loadPlugin(filePath);
        if(!plugin)
        {
            qDebug()<<"Error while loading plugin," << filePath;
        }else{
            qDebug()<<"plugin loaded," << filePath;
            mPlugins.push_back(plugin);

            if(fileInfo.baseName().toLower()=="default")
            {
                mDefaultPlugin = plugin;
                qDebug()<<"Default plugin ," << filePath;
            }
        }
    }

    return mPlugins.size();
}
