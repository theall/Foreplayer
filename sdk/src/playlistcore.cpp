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
#include "playlistcore.h"

#include "cplusutils.h"
#include <fstream>
#include <time.h>
#include <zlib.h>

#define PLAYLIST_DIR            L"playlist"
#define PLAYLIST_INDEX          L"index.json"
#define DEFAULT_PLUGIN_NAME     L"default"
#define SEC_PLAYLIST            "playlist"
#define SEC_CURRENT_PLAYLIST    "currentPlaylist"
#define SEC_CURRENT_MUSIC       "currentMusic"
#define SEC_CURRENT_TRACK       "currentTrack"

#define RecordMusicItem TMusicItem *currentItem = playlistItem->musicItem(mMusiclistIndex)
#define RestoreMusicItem mMusiclistIndex = playlistItem->indexOf(currentItem)

wstring TPlaylistCore::mPluginDir = PLAYLIST_DIR;

TPlaylistCore::TPlaylistCore() :
    mPlaylistIndex(-1),
    mMusiclistIndex(-1),
    mTracklistIndex(-1),
    mFileSaving(false)
{
    mCurrentDir = getCurrentDir() + L"/";
    mPlaylistDir = mCurrentDir + mPluginDir + L"/";
    if(!isFileExist(mPlaylistDir))
        createDir(mPlaylistDir);

    findPlaylist();
    if(mPlaylist.size()<=0)
        insert(DEFAULT_PLUGIN_NAME);

    time_t tm;
    srand(time(&tm));
}

TPlaylistCore::~TPlaylistCore()
{
    save();
}

void TPlaylistCore::setPlaylistDir(wstring dir)
{
    mPluginDir = dir;
}

vector<wstring> TPlaylistCore::names()
{
    vector<wstring> sl;
    mMutex.lock();
    for (TPlaylistItem *item : mPlaylist) {
        sl.push_back(item->name());
    }
    mMutex.unlock();

    return sl;
}

int TPlaylistCore::size()
{
    mMutex.lock();
    int pSize = mPlaylist.size();
    mMutex.unlock();
    return pSize;
}

TPlaylistItem *TPlaylistCore::currentPlaylistItem()
{
    TPlaylistItem *ret = NULL;
    mMutex.lock();
    if(mPlaylistIndex>=0 && mPlaylistIndex<(int)mPlaylist.size())
        ret = mPlaylist[mPlaylistIndex];
    mMutex.unlock();
    return ret;
}

int TPlaylistCore::insert(wstring name, int index)
{
    TPlaylistItem *playlistItem = new TPlaylistItem;
    playlistItem->setDisplayName(name);
    playlistItem->setFileName(getFileName());

    int ret = -1;
    mMutex.lock();

    TPlaylistItem *currentItem = NULL;
    if(mPlaylistIndex>=0 && mPlaylistIndex<(int)mPlaylist.size())
        currentItem = mPlaylist.at(mPlaylistIndex);

    if(index>-1)
    {
        mPlaylist.insert(mPlaylist.begin()+index, playlistItem);
        ret = index;
    } else {
        ret = mPlaylist.size();
        mPlaylist.push_back(playlistItem);
    }
    mMutex.unlock();

    if(currentItem)
        mPlaylistIndex = indexOf(currentItem);

    return ret;
}

bool TPlaylistCore::remove(int index)
{
    if(index<0 || index>=(int)mPlaylist.size())
        return false;

    mMutex.lock();
    TPlaylistItem *currentItem = mPlaylist.at(mPlaylistIndex);
    TPlaylistItem *item = mPlaylist.at(index);
    mPlaylist.erase(mPlaylist.begin()+index);
    mMutex.unlock();

    mPlaylistIndex = indexOf(currentItem);

    delete item;    
    return true;
}

void TPlaylistCore::rename(int index, wstring newName)
{
    if(index<0 || index>=(int)mPlaylist.size())
        return;

    TPlaylistItem *playlistItem = mPlaylist[index];
    playlistItem->setDisplayName(newName);
}

void TPlaylistCore::sort(SortMethod mode)
{
    mMutex.lock();
    TPlaylistItem *currentItem = mPlaylist.at(mPlaylistIndex);
    if(mode==SM_TITLE_ASC)
        std::sort(mPlaylist.begin(), mPlaylist.end(), [=](TPlaylistItem *a, TPlaylistItem *b){
            return lower(a->name()) > lower(b->name());
        });
    else if(mode==SM_TITLE_DES)
        std::sort(mPlaylist.begin(), mPlaylist.end(), [=](TPlaylistItem *a, TPlaylistItem *b){
            return lower(a->name()) < lower(b->name());
        });
    mMutex.unlock();

    mPlaylistIndex = indexOf(currentItem);
}

int TPlaylistCore::indexOf(TPlaylistItem *item)
{
    int i = 0;
    int ret = -1;
    mMutex.lock();
    for(TPlaylistItem *t : mPlaylist)
    {
        if(t==item)
        {
            ret = i;
            break;
        }
        i++;
    }
    mMutex.unlock();
    return ret;
}

int TPlaylistCore::playingPlaylistIndex()
{
    return mPlaylistIndex;
}

void TPlaylistCore::setPlayingPlaylistIndex(int index)
{
    mPlaylistIndex = index;
}

int TPlaylistCore::playingMusicIndex()
{
    return mMusiclistIndex;
}

void TPlaylistCore::setPlayingMusicListIndex(int index)
{
    mMusiclistIndex = index;
}

int TPlaylistCore::playingTrackIndex()
{
    return mTracklistIndex;
}

void TPlaylistCore::setPlayingTrackListIndex(int index)
{
    mTracklistIndex = index;
}

void TPlaylistCore::playingIndex(int *pIndex, int *mIndex, int *tIndex)
{
    if(mIndex==NULL || pIndex==NULL || tIndex==NULL)
        return;

    *pIndex = mPlaylistIndex;
    *mIndex = mMusiclistIndex;
    *tIndex = mTracklistIndex;
}

void TPlaylistCore::setPlayingIndex(int pIndex, int mIndex, int tIndex)
{
    mPlaylistIndex = pIndex;
    mMusiclistIndex = mIndex;
    mTracklistIndex = tIndex;
}

void TPlaylistCore::exportAs(int index, wstring fileName)
{
    (void)index;
    (void)fileName;
}

void TPlaylistCore::exportAll(wstring path)
{
    (void)path;
}

TMusicItem *TPlaylistCore::currentMusicItem()
{
    TPlaylistItem *playlistItem = currentPlaylistItem();
    if(!playlistItem)
        return NULL;

    return playlistItem->musicItem(mMusiclistIndex);
}

list<int> TPlaylistCore::removeRedundant(TPlaylistItem *playlistItem)
{
    list<int> ret;
    if(playlistItem)
    {
        RecordMusicItem;
        ret = playlistItem->removeRedundant();
        RestoreMusicItem;
    }

    return ret;
}

list<int> TPlaylistCore::removeErrors(TPlaylistItem *playlistItem)
{
    list<int> ret;
    if(playlistItem)
    {
        RecordMusicItem;
        ret = playlistItem->removeErrors();
        RestoreMusicItem;
    }

    return ret;
}

list<int> TPlaylistCore::removeMusicItems(TPlaylistItem *playlistItem, list<int> indexes)
{
    list<int> ret;
    indexes.sort();
    indexes.reverse();
    if(playlistItem)
    {
        RecordMusicItem;
        for(int index : indexes)
            if(playlistItem->remove(index))
                ret.push_back(index);
        RestoreMusicItem;
    }

    return ret;
}

list<int> TPlaylistCore::moveMusicItems(TPlaylistItem *playlistItem, list<int> indexes, int pos)
{
    list<int> ret;
    if(playlistItem)
    {
        RecordMusicItem;
        ret = playlistItem->move(indexes, pos);
        RestoreMusicItem;
    }

    return ret;
}

list<int> TPlaylistCore::insertMusicItems(TPlaylistItem *playlistItem, int pos, TMusicItems musicItems)
{
    list<int> ret;
    if(playlistItem)
    {
        RecordMusicItem;
        ret = playlistItem->insert(pos, musicItems);
        RestoreMusicItem;
    }

    return ret;
}

void TPlaylistCore::removeAllMusicItems(TPlaylistItem *playlistItem)
{
    if(playlistItem)
    {
        RecordMusicItem;
        playlistItem->clear();
        RestoreMusicItem;
    }
}

void TPlaylistCore::sortMusicItems(TPlaylistItem *playlistItem, SortMethod sm)
{
    if(playlistItem)
    {
        RecordMusicItem;
        playlistItem->sort(sm);
        RestoreMusicItem;
    }
}

TTrackItem *TPlaylistCore::currentTrackItem()
{
    TMusicItem *musicItem = currentMusicItem();
    if(!musicItem)
        return NULL;

    return musicItem->trackItem(mTracklistIndex);
}

TMusicItem *TPlaylistCore::parse(wstring file)
{
    TMusicItem *musicItem = new TMusicItem;
    TMusicInfo *musicInfo = new TMusicInfo;
    wstring fileBaseName = extractBaseName(file);
    musicInfo->musicName = fileBaseName;
    musicItem->setOriginalName(fileBaseName);
    musicItem->setDisplayName(fileBaseName);
    musicItem->setFileName(file);
    TBackendPlugin *plugin = TBackendPluginManager::instance()->parse(file, musicInfo);
    if(plugin)
    {
        // If this file is successful parsed, bind it to music item.
        musicItem->setDisplayName(musicInfo->musicName.c_str());
        musicItem->setOriginalName(musicItem->displayName());
        musicItem->setArtist(musicInfo->artist.c_str());
        musicItem->setYear(i2wstr(musicInfo->year));
        musicItem->setSystem(musicInfo->system.c_str());
        musicItem->setAdditionalInfo(musicInfo->additionalInfo.c_str());
        musicItem->setDuration(musicInfo->duration);
        musicItem->setSampleRate(musicInfo->sampleRate);
        musicItem->setChannels(musicInfo->channels);
        for (TTrackInfo *track : musicInfo->trackList) {
            TTrackItem *trackItem = new TTrackItem;
            trackItem->fileName = file;
            trackItem->displayName = track->trackName.c_str();
            trackItem->originalName = trackItem->displayName;
            trackItem->artist = track->artist.c_str();
            trackItem->year = i2wstr(track->year);
            trackItem->system = track->system.c_str();
            trackItem->additionalInfo = track->additionalInfo.c_str();
            trackItem->duration = track->duration;
            trackItem->sampleRate = track->sampleRate;
            trackItem->channels = track->channels;
            trackItem->originalDuration = trackItem->duration;
            trackItem->index = i2wstr(track->index);
            trackItem->indexName = track->indexName.c_str();
            if(trackItem->index.empty())
                trackItem->index = i2wstr(track->index);
            musicItem->addTrackItem(trackItem);
            delete track;
        }
    }
    delete musicInfo;
    return musicItem;
}

TPlaylistItem *TPlaylistCore::playlistItem(int plIndex)
{
    if(plIndex<0 || plIndex>=(int)mPlaylist.size())
        return NULL;

    return mPlaylist[plIndex];
}

TPlaylistItem *TPlaylistCore::takeAt(int plIndex)
{
    if(plIndex<0 || plIndex>=(int)mPlaylist.size())
        return NULL;

    TPlaylistItem *item = mPlaylist.at(plIndex);
    mPlaylist.erase(mPlaylist.begin()+plIndex);

    return item;
}

list<int> TPlaylistCore::move(list<int> indexes, int pos)
{
    mMutex.lock();
    TPlaylistItem *playingItem = mPlaylist.at(mPlaylistIndex);

    int listSize = mPlaylist.size();
    TPlaylistItems items;
    list<int> indexesMoved;
    if(indexes.size()<=0 || listSize<=0)
        return indexesMoved;

    if(pos < 0)
        pos = 0;
    if(pos > listSize)
        pos = listSize;

    indexes.sort();
    indexes.reverse();

    for (int i : indexes) {
        if(i<0 || i>=listSize)
            continue;
        items.push_back(takeAt(i));
        if(i < pos)
            pos--;
    }
    int i = 0;
    for (TPlaylistItem *item : items) {
        insert(pos, item);
        indexesMoved.push_back(pos+i++);
    }

    mMutex.unlock();

    mPlaylistIndex = indexOf(playingItem);

    return indexesMoved;
}

int TPlaylistCore::insert(int pos, TPlaylistItem *item)
{
    mPlaylist.insert(mPlaylist.begin()+pos, item);

    if(pos < 0)
        pos = 0;
    else if (pos >= (int)mPlaylist.size())
        pos = mPlaylist.size()-1;

    return pos;
}

void TPlaylistCore::findPlaylist()
{
    wstring indexFilePath = mPlaylistDir + PLAYLIST_INDEX;
    ifstream i(wstring2string(indexFilePath));
    json jsonwrap;
    try {
        i >> jsonwrap;
    } catch(...) {
        return;
    }
    json fileNameList = jsonwrap[SEC_PLAYLIST];
    mPlaylistIndex = jsonwrap[SEC_CURRENT_PLAYLIST];
    mMusiclistIndex = jsonwrap[SEC_CURRENT_MUSIC];
    mTracklistIndex = jsonwrap[SEC_CURRENT_TRACK];
    for(json fileNameObject : fileNameList)
    {
        wstring fileName = fileNameObject.get<wstring>();
        loadPlaylist(fileName);
    }
    int size = mPlaylist.size();
    if(mPlaylistIndex<0 || mPlaylistIndex>=size)
    {
        mPlaylistIndex = 0;
        mMusiclistIndex = 0;
        mTracklistIndex = 0;
    } else {
        TPlaylistItem *playlistItem = mPlaylist[mPlaylistIndex];
        TMusicItem *musicItem = playlistItem->musicItem(mMusiclistIndex);
        if(!musicItem)
        {
            mMusiclistIndex = 0;
            mTracklistIndex = 0;
        } else {
            if(!musicItem->trackItem(mTracklistIndex))
                mTracklistIndex = 0;
        }
    }
}

void TPlaylistCore::loadPlaylist(wstring fileName)
{
    uint8_t *buf;
    long size;
    readFile(fileName, &buf, &size);
    if(!buf)
        return;

    uLongf uncompressSize = *(int*)buf;
    uint8_t *destBuf = new uint8_t[uncompressSize];
    if(!destBuf)
    {
        delete buf;
        return;
    }
    int ret = uncompress(destBuf, &uncompressSize, buf+sizeof(int), size-sizeof(int));
    delete buf;
    if(ret!=Z_OK)
        return;

    vector<uint8_t> v_cbor(destBuf, destBuf+uncompressSize);
    json jsonObject;
    try {
        jsonObject = json::from_cbor(v_cbor);
    } catch(...) {
        return;
    }

    delete destBuf;

    try{
        TPlaylistItem *playlistItem = new TPlaylistItem(fileName);
        playlistItem->fromJson(jsonObject);

        mPlaylist.push_back(playlistItem);
    } catch(...) {

    }
}

void TPlaylistCore::save()
{
    if(mFileSaving)
        return;

    if(!isDirExist(mCurrentDir))
        createDir(mCurrentDir);

    json playlist;
    for (TPlaylistItem *playlistItem : mPlaylist) {
        playlistItem->save();
        playlist.push_back(playlistItem->fileName());
    }

    mFileSaving = true;

    ofstream indexFile(wstring2string((mPlaylistDir+PLAYLIST_INDEX)));
    json j;

    j[SEC_PLAYLIST] = playlist;
    j[SEC_CURRENT_PLAYLIST] = mPlaylistIndex;
    j[SEC_CURRENT_MUSIC] = mMusiclistIndex;
    j[SEC_CURRENT_TRACK] = mTracklistIndex;

    indexFile << j;

    mFileSaving = false;
}

wstring TPlaylistCore::getFileName()
{
    wstring fileName;
    int listSize = mPlaylist.size();
    for(int i=0;i<listSize+1;i++)
    {
        fileName = mPlaylistDir + i2wstr(i);
        bool existed = false;
        for(int j=0;j<listSize;j++)
        {
            if(mPlaylist[j]->fileName() == fileName)
            {
                existed = true;
                break;
            }
        }
        if(!existed)
            break;
    }

    assert(!fileName.empty());

    return fileName;
}
