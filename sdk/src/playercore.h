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
#ifndef TPLAYERCORE_H
#define TPLAYERCORE_H

#include "playthread.h"

#include "front/sdlfront.h"

#include "pluginmanager/backendpluginmanager.h"
#include "playlist/playlistitem.h"

enum PlayerState
{
    PS_PAUSED,
    PS_STOPED,
    PS_PLAYING
};

typedef void (*TPlayCallback)(int played, int total, void *data);

class TPlayerCore
{
public:
    TPlayerCore();
    ~TPlayerCore();

    TMusicInfo *parse(wstring fileName);

    bool playTrack(TTrackItem *track);

    void stop();
    void pause();
    bool resume();
    bool isPaused();
    bool isStoped();
    bool isPlaying();

    int playedTime();

    void setAudioParameter(AudioParameter type, float value, int param=0);
    void getAudioData(AudioDataType dataType, void *param1, void* param2);

private:
    TPlayThread *mPlayThread;
    TBackendPluginManager *mPluginManager;
    PlayerState mPlayerState;

    void init();
    void destroyPlayThread();
};

#endif // TPLAYERCORE_H
