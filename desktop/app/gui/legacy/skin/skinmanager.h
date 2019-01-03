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
#ifndef TSKINMANAGER_H
#define TSKINMANAGER_H

#include <pch.h>
#include "skin.h"

typedef QList<TSkin*> TSkins;
class TSkinManager
{
public:
    TSkinManager();

    void setPath(QString path);
    void reload();
    QStringList skinNames();
    TSkin *skinAt(int i);
    TSkin *findSkin(QString skinFullPath);
    int indexOf(QString skinFullPath);
    int size();

private:
    QString mSkinPath;
    TSkins mSkins;
    QStringList mNameFilter;
};
#endif // TSKINMANAGER_H
