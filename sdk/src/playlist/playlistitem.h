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
#ifndef TPLAYLISTITEM_H
#define TPLAYLISTITEM_H

#include "musicitem.h"

#include <list>

class TPlaylistItem
{
public:
    TPlaylistItem(wstring fileName = wstring());
    ~TPlaylistItem();

    json toJson();
    void fromJson(json object);

    int size();
    TMusicItem *takeAt(int index);
    void insert(int pos, TMusicItem *item);
    void update(int index, TMusicItem *item);
    bool remove(int index);
    list<int> removeRedundant();
    list<int> removeErrors();
    void rename(int index, wstring newName);
    void sort(SortMethod mode = SM_TITLE_ASC);
    int indexOf(TMusicItem *item);

    wstring name() { return mDisplayName; }
    void setDisplayName(wstring newName);

    TMusicItem *musicItem(int index);
    int musicItemIndex(TMusicItem *musicItem);

    wstring fileName() { return mFileName; }
    void setFileName(wstring fileName);

    bool isModified() { return mModified; }

    int version() { return mVersion; }

    void save();
    void clear();

private:
    wstring mDisplayName;
    TMusicItems mMusicItems;
    wstring mFileName;
    bool mModified;
    int mVersion;
};

typedef vector<TPlaylistItem*> TPlaylistItems;

#endif // TPLAYLISTITEM_H
