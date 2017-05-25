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

#include "playlistitem.h"

#include "playlistdef.h"

#include "../cplusutils.h"

#include <set>
#include <algorithm>
#include <zlib.h>

TPlaylistItem::TPlaylistItem(wstring fileName) :
    TAbstractItem()
  , mFileName(fileName)
  , mModified(false)
{
}

TPlaylistItem::~TPlaylistItem()
{
    clear();
}

json TPlaylistItem::toJson()
{
    json object;
    //Set the data of the playlist.
    object[K_NAME] = mDisplayName;
    object[K_FILE] = mFileName;
    object[K_VERSION] = mVersion;

    json musicsArray;
    for (TMusicItem *musicItem : mMusicItems) {
        musicsArray.push_back(musicItem->toJson());
    }
    object[K_MUSICLIST] = musicsArray;
    return object;
}

void TPlaylistItem::fromJson(json object)
{
    mDisplayName = object[K_NAME].get<wstring>();
    mVersion = object[K_VERSION];
    json musicArray = object[K_MUSICLIST];
    for (json musicObject : musicArray) {
        TMusicItem *musicItem = new TMusicItem;
        musicItem->fromJson(musicObject);
        mMusicItems.push_back(musicItem);
    }
}

TMusicItem *TPlaylistItem::takeAt(int index)
{
    if(index<0 || index>=(int)mMusicItems.size())
        return NULL;

    mModified = true;
    TMusicItem *musicItem = mMusicItems.at(index);
    mMusicItems.erase(mMusicItems.begin()+index);
    return musicItem;
}

int TPlaylistItem::insert(int pos, TMusicItem *item)
{
    if(!item)
        return -1;

    int listSize = mMusicItems.size();
    if(pos < 0)
        pos = 0;
    if(pos > listSize)
        pos = listSize;

    mMusicItems.insert(mMusicItems.begin()+pos, item);
    mModified = true;

    return pos;
}

list<int> TPlaylistItem::insert(int pos, TMusicItems musicItems)
{
    int listSize = mMusicItems.size();
    if(pos < 0)
        pos = 0;
    if(pos > listSize)
        pos = listSize;

    int i = 0;
    list<int> newIndexes;
    for(TMusicItem *musicItem : musicItems) {
        if(musicItem)
        {
            insert(pos, musicItem);
            newIndexes.push_back(pos + i++);
        }
    }
    return newIndexes;
}

list<int> TPlaylistItem::move(list<int> indexes, int pos)
{
    int listSize = mMusicItems.size();
    if(pos < 0)
        pos = 0;
    if(pos > listSize)
        pos = listSize;

    indexes.sort();
    indexes.reverse();

    TMusicItems musicItems;
    for (int i : indexes) {
        if(i<0 || i>=listSize)
            continue;
        musicItems.push_back(takeAt(i));
        if(i < pos)
            pos--;
    }
    int i = 0;
    list<int> indexesMoved;
    for (TMusicItem *item : musicItems) {
        insert(pos, item);
        indexesMoved.push_back(pos+i++);
    }
    return indexesMoved;
}

void TPlaylistItem::update(int index, TMusicItem *item)
{
    if(index<0 || index>=(int)mMusicItems.size())
        return;

    TMusicItem *oldItem = mMusicItems[index];
    mMusicItems[index] = item;
    delete oldItem;
    mModified = true;
}

bool TPlaylistItem::remove(int index)
{
    if(index<0 || index>=(int)mMusicItems.size())
        return false;

    delete mMusicItems[index];
    mMusicItems.erase(mMusicItems.begin()+index);
    mModified = true;
    return true;
}

list<int> TPlaylistItem::remove(list<int> indexes)
{
    indexes.sort();
    indexes.reverse();

    int musicItemsSize = (int)mMusicItems.size();
    list<int> ret;
    for(int i : indexes)
    {
        if(i<0 || i>=musicItemsSize)
            continue;
        delete mMusicItems[i];
        mMusicItems.erase(mMusicItems.begin()+i);
        ret.push_back(i);
    }
    mModified = true;

    return ret;
}

list<int> TPlaylistItem::removeRedundant()
{
    set<int> removedSet;
    int itemSize = mMusicItems.size();
    for(int i=0;i<itemSize;i++)
    {
        TMusicItem *item1 = mMusicItems[i];
        for(int j=i+1;j<itemSize;j++)
        {
            TMusicItem *item2 = mMusicItems[j];
            if(lower(item1->fileName()) == lower(item2->fileName()))
            {
                removedSet.insert(j);
            }
        }
    }
    list<int> removedList(removedSet.begin(), removedSet.end());
    int removedSize = removedList.size();
    if(removedSize > 0)
    {
        removedList.sort();
        removedList.reverse();

        for(int i : removedList) {
            delete mMusicItems[i];
            mMusicItems.erase(mMusicItems.begin()+i);
        }
        mModified = true;
    }
    return removedList;
}

list<int> TPlaylistItem::removeErrors()
{
    list<int> removed;
    int i=0;
    for(TMusicItem *musicItem : mMusicItems)
    {
        if(!isFileExist(musicItem->fileName()) || (musicItem->duration()<=0 && musicItem->size()<=0))
            removed.push_back(i);
        i++;
    }

    int removedSize = removed.size();
    if(removedSize > 0)
    {
        removed.sort();
        removed.reverse();
        for(int index : removed)
        {
            delete mMusicItems[index];
            mMusicItems.erase(mMusicItems.begin()+index);
        }
        mModified = true;
    }
    return removed;
}

bool TPlaylistItem::setDisplayName(wstring newName)
{
    if(mDisplayName != newName)
    {
        mDisplayName = newName;
        mModified = true;
        return true;
    }
    return false;
}

void TPlaylistItem::sort(SortMethod mode)
{
    if(mode==SM_TITLE_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return lower(a->displayName()) < lower(b->displayName());
        });
    else if (mode==SM_TITLE_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return lower(a->displayName()) > lower(b->displayName());
        });
    else if (mode==SM_LENGTH_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName().size() < b->displayName().size();
        });
    else if (mode==SM_LENGTH_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName().size() > b->displayName().size();
        });
    else if (mode==SM_DURATION_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->duration() < b->duration();
        });
    else if (mode==SM_DURATION_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->duration() > b->duration();
        });
    else if (mode==SM_FILE_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->fileName() < b->fileName();
        });
    else if (mode==SM_FILE_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->fileName() > b->fileName();
        });
    else if (mode==SM_ARTIST_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->artist() < b->artist();
        });
    else if (mode==SM_ARTIST_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->artist() > b->artist();
        });
    else if (mode==SM_GAME_ASC || mode==SM_ALBUM_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->game() < b->game();
        });
    else if (mode==SM_GAME_DES || mode==SM_ALBUM_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->game() > b->game();
        });
    else if (mode==SM_SYSTEM_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->system() < b->system();
        });
    else if (mode==SM_SYSTEM_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->system() > b->system();
        });
    else if (mode==SM_YEAR_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return _wtoi(a->year().c_str()) < _wtoi(b->year().c_str());
        });
    else if (mode==SM_YEAR_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return _wtoi(a->year().c_str()) > _wtoi(b->year().c_str());
        });
    else if (mode==SM_RANDOM)
    {
        // Dose this code have problem?This may be a bug of stl.
//        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
//            (void)a;
//            (void)b;
//            return rand() < rand();
//        });

        int size = mMusicItems.size();
        for(int i=0;i<size;i++)
        {
            int i1 = ((float)rand()/RAND_MAX)*(size-1);
            int i2 = ((float)rand()/RAND_MAX)*(size-1);
            TMusicItem *temp = mMusicItems[i1];
            mMusicItems[i1] = mMusicItems[i2];
            mMusicItems[i2] = temp;
        }
    } else if (mode==SM_REVERSE) {
        std::reverse(mMusicItems.begin(), mMusicItems.end());
    }
}

int TPlaylistItem::indexOf(TMusicItem *item)
{
    int i = 0;
    for(TMusicItem *musicItem : mMusicItems)
    {
        if(item==musicItem)
            return i;
        i++;
    }
    return -1;
}

TMusicItem *TPlaylistItem::musicItem(int index)
{
    if(index<0 || index>=(int)mMusicItems.size())
        return NULL;

    return mMusicItems[index];
}

int TPlaylistItem::musicItemIndex(TMusicItem *musicItem)
{
    return indexOf(musicItem);
}

void TPlaylistItem::setFileName(wstring fileName)
{
    if(mFileName != fileName)
    {
        mFileName = fileName;
        mModified = true;
    }
}

void TPlaylistItem::save()
{
    bool modified = mModified;
    for(TMusicItem *item : mMusicItems)
    {
        modified |= item->isModified();
    }
    if(modified)
    {
        FILE *fp = _wfopen(mFileName.c_str(), L"wb");
        if(!fp)
            return;

        json j = toJson();
        vector<uint8_t> v_cbor = json::to_cbor(j);
        int fileSize = v_cbor.size();
        if(fileSize > 0)
        {
            uLongf compressedSize = compressBound(fileSize);
            if(uint8_t *buf = new uint8_t[compressedSize])
            {
                if(compress(buf, &compressedSize, &v_cbor[0], fileSize)==Z_OK)
                {
                    fwrite(&fileSize, sizeof(int), 1, fp);
                    fwrite(buf, sizeof(uint8_t), compressedSize, fp);
                }
                delete buf;
            }
        }
        //Close the file.
        fclose(fp);

        for(TMusicItem *item : mMusicItems)
        {
            item->setModified(false);
        }
        mModified = false;
    }
}

void TPlaylistItem::clear()
{
    if(mMusicItems.size() <= 0)
        return;

    for (TMusicItem *item : mMusicItems) {
        delete item;
    }
    mMusicItems.clear();
    mModified = true;
}

int TPlaylistItem::size()
{
    return mMusicItems.size();
}
