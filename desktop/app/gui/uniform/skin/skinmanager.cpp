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
#include "skinmanager.h"
#include "utils.h"

TSkinManager::TSkinManager()
{
    mNameFilter << "*.zip";
    setPath("skins");
}

void TSkinManager::setPath(QString path)
{
    mSkinPath = Utils::absoluteFilePath(path);
    reload();
}

void TSkinManager::reload()
{
    for(TSkin *skin : mSkins)
        delete skin;

    mSkins.clear();

    QStringList skinFullNames;
    QDir dir(mSkinPath);
    QStringList fileList = dir.entryList(mNameFilter);
    for(auto file : fileList)
        skinFullNames.append(dir.absoluteFilePath(file));

    fileList = dir.entryList(QDir::Dirs|QDir::NoDot|QDir::NoDotDot);
    for(auto file : fileList)
        skinFullNames.append(dir.absoluteFilePath(file+"/skin.xml"));

    for(QString fileName : skinFullNames)
    {
        TSkin *skin = new TSkin;
        if(!skin->load(fileName))
        {
            delete skin;
            continue;
        }
        mSkins.append(skin);
    }
}

QStringList TSkinManager::skinNames()
{
    QStringList names;
    for(TSkin *skin : mSkins)
    {
        names.append(skin->name());
    }
    return names;
}

TSkin *TSkinManager::skinAt(int i)
{
    if(i>=0 && i<mSkins.size())
        return mSkins[i];
    return NULL;
}

TSkin *TSkinManager::findSkin(QString skinFullPath)
{
    for(TSkin *skin : mSkins)
        if(skin->fileName() == skinFullPath)
            return skin;

    return NULL;
}

int TSkinManager::indexOf(QString skinFullPath)
{
    for(int i=0;i<mSkins.size();i++)
    {
        if(mSkins[i]->fileName() == skinFullPath)
            return i;
    }
    return -1;
}

int TSkinManager::size()
{
    return mSkins.size();
}
