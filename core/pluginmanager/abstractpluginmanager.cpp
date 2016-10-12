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

#include <dir.h>

TAbstractPluginManager::TAbstractPluginManager(QObject *parent) :
    QObject(parent)
{
}

int TAbstractPluginManager::findPlugins(QString path)
{
    QString abPath = QDir::currentPath() + "/plugins/" + path;
    QDir dir(abPath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString filePath = fileInfo.filePath();
        TAbstractPlugin* plugin = loadPlugin(filePath);
        if(!plugin)
        {
            qDebug()<<"Error while loading plugin,"<<filePath;
        }else{
            qDebug()<<"Load plugin,"<<filePath;
            mPlugins.append(plugin);
        }
    }

    return mPlugins.size();
}
