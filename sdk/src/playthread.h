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

#ifndef TPLAYTHREAD_H
#define TPLAYTHREAD_H

#include "front/abstractfront.h"
#include "pluginmanager/backend/backendplugin.h"

#include <thread>

enum TThreadState
{
    TS_NULL,
    TS_READY,
    TS_PAUSED,
    TS_RUNNING,
    TS_TERMINATE
};

class TPlayThread
{
public:
    explicit TPlayThread();
    ~TPlayThread();

    void start();

    void pause();
    void stop();
    void play();
    void needToTerminate();

    int playedTime();

    void setBackend(TBackendPlugin *plugin);

    bool isPaused();

    void setAudioParameter(AudioParameter type, float value, int param);
    void getAudioData(AudioDataType dataType, void *param1, void* param2);

protected:
    void run();

private:
    bool mNeedTerminate;
    int mCurrentMicroSeconds;
    TThreadState mState;

    TAbstractFront *mFront;
    TBackendPlugin *mBackendPlugin;

    thread *mThread;
};
#endif // TPLAYTHREAD_H
