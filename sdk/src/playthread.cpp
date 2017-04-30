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

#include "playthread.h"

#include "front/sdlfront.h"

#define CYCLE_INTERVAL      300

TPlayThread::TPlayThread() :
    mNeedTerminate(false),
    mCurrentMicroSeconds(0),
    mState(TS_NULL),
    mFront(NULL),
    mBackendPlugin(NULL),
    mThread(NULL)
{

}

TPlayThread::~TPlayThread()
{
    if(mThread)
    {
        mNeedTerminate = true;
        delete mThread;
        mThread = NULL;
    }
}

void TPlayThread::start()
{
    if(mThread == NULL)
    {
        mThread = new thread(&TPlayThread::run, this);
        mThread->detach();
    }
}

void TPlayThread::pause()
{
    if(mFront)
    {
        mFront->pause();
        mState = TS_PAUSED;
    } else {
        mState = TS_READY;
    }
}

void TPlayThread::stop()
{
    if(mFront)
        mFront->stop();

    mState = TS_READY;
}

void TPlayThread::play()
{
    if(mState != TS_PAUSED)
        mCurrentMicroSeconds = 0;

    if(mFront)
    {
        mFront->play();
        mState = TS_RUNNING;
    } else {
        mState = TS_READY;
    }
}

void TPlayThread::needToTerminate()
{
    mNeedTerminate = true;
}

bool TPlayThread::seek(int ms)
{
    bool ret = false;
    if(mBackendPlugin)
        ret = mBackendPlugin->seek(ms);

    if(ret)
        mCurrentMicroSeconds = ms;

    return ret;
}

int TPlayThread::playedTime()
{
    return mCurrentMicroSeconds;
}

void TPlayThread::setBackend(TBackendPlugin *plugin)
{
    if(mBackendPlugin == plugin)
        return;

    if(mBackendPlugin)
        mBackendPlugin->closeTrack();

    mBackendPlugin = plugin;
    if(mFront)
    {
        if(plugin)
        {
            mFront->setBackendPlugin(plugin);
        } else {
            mFront->stop();
        }

    }
}

bool TPlayThread::isPaused()
{
    return mState==TS_PAUSED;
}

void TPlayThread::setAudioParameter(AudioParameter type, float value, int param)
{
    if(mFront)
        mFront->setAudioParameter(type, value, param);
}

void TPlayThread::getAudioData(AudioDataType dataType, void *param1, void *param2)
{
    if(mFront)
        mFront->getAudioData(dataType, param1, param2);
}

void TPlayThread::run()
{
    if(!mFront)
        mFront = new TSDLFront;

    mFront->start();

    mState = TS_READY;
    while(!mNeedTerminate)
    {
        this_thread::sleep_for((chrono::milliseconds(CYCLE_INTERVAL)));

        if(mState==TS_RUNNING)
            mCurrentMicroSeconds += CYCLE_INTERVAL;
    }

    mFront->stop();
    mState = TS_TERMINATE;
}
