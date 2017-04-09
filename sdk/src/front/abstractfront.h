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
#ifndef TABSTRACTFRONT
#define TABSTRACTFRONT

#include <foreplayer.h>

#include "../filter/samplesfilter.h"
#include "../pluginmanager/backend/backendplugin.h"

#include "loopbuffer.h"

#define SAMPLE_RATE         44100
#define SOUND_SEGMENTS      48
#define SOUND_FPS           600

class TAbstractFront : public IDataCallback
{
public:
    TAbstractFront();
    ~TAbstractFront();

    virtual void setRecorder() = 0;
    virtual void setFilter() = 0;

    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual void step() = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void setSampleSize(int sampleSize);

    void setBackendPlugin(TBackendPlugin *plugin);
    void requestNextSamples(int bufSize, char *samples);

    bool isPlaying() { return mPlaying; }
    int sampleCount();

    void setAudioParameter(AudioParameter type, float value, int param);
    void getAudioData(AudioDataType dataType, void *param1, void* param2);

protected:
    bool mAudioEnabled;
    int mSamplesCount;
    short *mSamplesBuf;
    bool mPlaying;

private:
    TSamplesFilter *mFilter;
    TBackendPlugin *mBackendPlugin;
    TLoopBuffer *mLoopBuf;

    // IDataCallback interface
private:
    void read(byte *buf, int size) override;
};

#endif // TABSTRACTFRONT
