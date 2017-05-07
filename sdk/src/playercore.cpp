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
#include "playercore.h"

TPlayerCore::TPlayerCore() :
    mPlayThread(NULL)
  , mPluginManager(TBackendPluginManager::instance())
  , mPlayerState(PS_STOPED)
  , mLastTrackItem(NULL)
{
    init();
}

TPlayerCore::~TPlayerCore()
{
    destroyPlayThread();
}

bool TPlayerCore::playTrack(TTrackItem *trackItem)
{
    if(!mPlayThread)
        return false;

    mPlayerState = PS_STOPED;
    mPlayThread->stop();

    if(!trackItem || !mPluginManager)
        return false;

    TTrackInfo trackInfo;
    trackInfo.index = _wtoi(trackItem->index.c_str());
    trackInfo.indexName = trackItem->indexName;
    trackInfo.musicFileName = trackItem->fileName;

    bool result = false;
    // Find a backend plugin which can process this track
    TBackendPlugin *plugin = mPluginManager->loadTrack(&trackInfo);
    if(plugin)
    {
        // Connect plugin's callback to front's callback
        mPlayThread->setBackend(plugin);
        result = true;
        mPlayThread->play();
        mPlayerState = PS_PLAYING;
        mLastTrackItem = trackItem;
    }

    return result;
}

void TPlayerCore::init()
{
    if(mPlayThread)
    {
        mPlayThread->play();
        mPlayerState = PS_PLAYING;
    }
    else
    {
        mPlayThread = new TPlayThread;
        mPlayThread->start();
    }
}

void TPlayerCore::stop()
{
    if(mPlayThread)
        mPlayThread->pause();

    mPlayerState = PS_STOPED;
}

void TPlayerCore::pause()
{
    if(mPlayThread)
    {
        mPlayThread->pause();
        mPlayerState = PS_PAUSED;
    }
}

int TPlayerCore::playedTime()
{
    if(mPlayThread)
    {
        return mPlayThread->playedTime();
    }

    return 0;
}

void TPlayerCore::getAudioData(AudioDataType dataType, void *param1, void* param2)
{
    if(mPlayThread)
    {
        mPlayThread->getAudioData(dataType, param1, param2);
    }
}

void TPlayerCore::setAudioParameter(AudioParameter type, float value, int param)
{
    if(mPlayThread)
        mPlayThread->setAudioParameter(type, value, param);
}

bool TPlayerCore::resume()
{
    if(mPlayThread && mPlayThread->isPaused())
    {
        mPlayThread->play();
        mPlayerState = PS_PLAYING;
        return true;
    }
    return false;
}

bool TPlayerCore::isPaused()
{
    return mPlayerState==PS_PAUSED && (mPlayThread && mPlayThread->isPaused());
}

bool TPlayerCore::isStoped()
{
    return mPlayerState==PS_STOPED || !mPlayThread;
}

bool TPlayerCore::isPlaying()
{
    return mPlayerState==PS_PLAYING;
}

bool TPlayerCore::seek(int ms)
{
    return mPlayThread?mPlayThread->seek(ms):false;
}

void TPlayerCore::destroyPlayThread()
{
    if(mPlayThread)
    {
        stop();
        delete mPlayThread;
        mPlayThread = NULL;
    }
}

