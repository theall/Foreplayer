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

#include "musicitem.h"

#include "playlistdef.h"
#include "../cplusutils.h"

#include <algorithm>

TMusicItem::TMusicItem() :
    TAbstractItem()
    , mDuration(0)
    , mFileSize(0)
    , mModified(false)
{
    time(&mLastParsed);
}

TMusicItem::~TMusicItem()
{
    clear();
}

json TMusicItem::toJson()
{
    json object;
    //Set the data of the playlist.
    object[K_NAME] = mDisplayName;
    object[K_FILE] = mFileName;
    object[K_ADDITIONAL] = mAdditionalInfo;
    object[K_DURATION] = mDuration;
    object[K_SIZE] = mFileSize;
    object[K_LAST_PARSED] = mLastParsed;

    json tracksArray;
    for (TTrackItem *trackItem : mTrackItems) {
        tracksArray.push_back(trackItem->toJson());
    }

    object[K_TRACKLIST] = tracksArray;
    return object;
}

void TMusicItem::fromJson(json object)
{
    mDisplayName = object[K_NAME].get<wstring>();
    mFileName = object[K_FILE].get<wstring>();
    mFileSize = object[K_SIZE];
    mDuration = object[K_DURATION];
    mAdditionalInfo = object[K_ADDITIONAL].get<wstring>();
    mLastParsed = object[K_LAST_PARSED];
    json trackArray = object[K_TRACKLIST];
    for (json trackObject : trackArray) {
        TTrackItem *trackItem = new TTrackItem;
        trackItem->fromJson(trackObject);
        mTrackItems.push_back(trackItem);
    }
}

void TMusicItem::fromTrackItem(TTrackItem *trackItem)
{
    if(!trackItem)
        return;

    mDisplayName = trackItem->displayName;
    mOriginalName = trackItem->originalName;
    mFileName = trackItem->fileName;
    mSystem = trackItem->system;
    mArtist = trackItem->artist;
    mYear = trackItem->year;
    mAdditionalInfo = trackItem->additionalInfo;
    mDuration = trackItem->duration;
    mModified = true;
}

void TMusicItem::sort(SortMethod mode)
{
    if(mode==SM_TITLE_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return lower(a->displayName) < lower(b->displayName);
        });
    else if (mode==SM_TITLE_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return lower(a->displayName) > lower(b->displayName);
        });
    else if (mode==SM_LENGTH_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.size() < b->displayName.size();
        });
    else if (mode==SM_LENGTH_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.size() > b->displayName.size();
        });
    else if (mode==SM_DURATION_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration < b->duration;
        });
    else if (mode==SM_DURATION_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration > b->duration;
        });
    else if (mode==SM_INDEX_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index < b->index;
        });
    else if (mode==SM_INDEX_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index > b->index;
        });
    else if (mode==SM_RANDOM)
    {
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            (void)a;
            (void)b;
            return rand() < rand();
        });
    }
}

wstring TMusicItem::displayName()
{
    return mDisplayName;
}

bool TMusicItem::setDisplayName(wstring displayName)
{
    if(mDisplayName != displayName)
    {
        mDisplayName = displayName;
        mModified = true;
        return true;
    }
    return false;
}

wstring TMusicItem::originalName()
{
    return mOriginalName;
}

void TMusicItem::setOriginalName(wstring name)
{
    mOriginalName = name;
}

wstring TMusicItem::fileName()
{
    return mFileName;
}

void TMusicItem::setFileName(wstring fileName)
{
    if(mFileName != fileName)
    {
        mFileName = fileName;
        mModified = true;
    }
}

wstring TMusicItem::system()
{
     return mSystem;
}

void TMusicItem::setSystem(wstring system)
{
     if(mSystem == system)
          return;

     mSystem = system;
     mModified = true;
}

wstring TMusicItem::artist()
{
     return mArtist;
}

void TMusicItem::setArtist(wstring artist)
{
     if(mArtist == artist)
          return;

     mArtist = artist;
     mModified = true;
}

wstring TMusicItem::year()
{
     return mYear;
}

void TMusicItem::setYear(wstring year)
{
     if(mYear == year)
          return;

     mYear = year;
     mModified = true;
}

wstring TMusicItem::additionalInfo()
{
    return mAdditionalInfo;
}

void TMusicItem::setAdditionalInfo(wstring info)
{
    if(mAdditionalInfo==info)
        return;

    mAdditionalInfo = info;
    mModified = true;
}

int TMusicItem::duration()
{
    return mDuration;
}

void TMusicItem::setDuration(int duration)
{
    if(mDuration != duration)
    {
        mDuration = duration;
        mModified = true;
    }
}

int TMusicItem::fileSize()
{
    return mFileSize;
}

void TMusicItem::setFileSize(int fileSize)
{
    if(mFileSize != fileSize)
    {
        mFileSize = fileSize;
        mModified = true;
    }
}

wstring TMusicItem::game()
{
    return mGame;
}

void TMusicItem::setGame(wstring game)
{
    if(mGame != game)
    {
        mGame = game;
        mModified = true;
    }
}

wstring TMusicItem::album()
{
    return mGame;
}

void TMusicItem::setAlbum(wstring album)
{
    if(mGame != album)
    {
        mGame = album;
        mModified = true;
    }
}

time_t TMusicItem::lastParsed()
{
    return mLastParsed;
}

void TMusicItem::setLastParsed(time_t lastParsed)
{
    if(mLastParsed != lastParsed)
    {
        mLastParsed = lastParsed;
        mModified = true;
    }
}

TTrackItems *TMusicItem::trackItems()
{
    return &mTrackItems;
}

TTrackItem *TMusicItem::trackItem(int index)
{
    if(index<0 || index>=(int)mTrackItems.size())
        return NULL;

    return mTrackItems.at(index);
}

int TMusicItem::indexOf(TTrackItem *trackItem)
{
    int i = 0;
    for(TTrackItem *item : mTrackItems)
    {
        if(item==trackItem)
            return i;
        i++;
    }
    return -1;
}

void TMusicItem::addTrackItem(TTrackItem *trackItem, int pos)
{
    if(pos == -1)
        mTrackItems.push_back(trackItem);
    else
        mTrackItems.insert(mTrackItems.begin()+pos, trackItem);
}

bool TMusicItem::isModified()
{
    return mModified;
}

void TMusicItem::setModified(bool modified)
{
    mModified = modified;
}

int TMusicItem::size()
{
    return mTrackItems.size();
}

void TMusicItem::clear()
{
    if(mTrackItems.size() <= 0)
        return;

    for (TTrackItem *item : mTrackItems)
    {
        delete item;
    }
    mTrackItems.clear();
    mModified = true;
}
