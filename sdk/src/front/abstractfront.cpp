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

#include "abstractfront.h"

#include <memory.h>

TAbstractFront::TAbstractFront() :
    mAudioEnabled(true),
    mSamplesCount(0),
    mSamplesBuf(NULL),
    mPlaying(false),
    mFilter(new TSamplesFilter(SAMPLE_RATE)),
    mBackendPlugin(NULL),
    mLoopBuf(new TLoopBuffer)
{

}

TAbstractFront::~TAbstractFront()
{
    if(mFilter)
    {
        delete mFilter;
        mFilter = NULL;
    }
    if(mLoopBuf)
    {
        delete mLoopBuf;
        mLoopBuf = NULL;
    }
}

void TAbstractFront::setSampleSize(int sampleSize)
{
    if(mLoopBuf)
        mLoopBuf->setCallback(this, sampleSize);
}

void TAbstractFront::setBackendPlugin(TBackendPlugin *plugin)
{
    setSampleSize(plugin->getSampleSize(SAMPLE_RATE, SOUND_FPS));
    mBackendPlugin = plugin;
}

void TAbstractFront::requestNextSamples(int bufSize, char *samples)
{
    if(mLoopBuf)
    {
        int samplesBufSize = mSamplesCount * 2 * sizeof(short);
        if(!mSamplesBuf || bufSize>samplesBufSize)
        {
            samplesBufSize = bufSize;
            free(mSamplesBuf);
            mSamplesBuf = (short*)malloc(samplesBufSize);
        }
        mSamplesCount = samplesBufSize / 2 / sizeof(short);
        memset(mSamplesBuf, 0, samplesBufSize);
        mLoopBuf->read((byte*)mSamplesBuf, samplesBufSize);
        mFilter->filter(mSamplesCount, mSamplesBuf);
        if(mAudioEnabled)
            memcpy(samples, mSamplesBuf, samplesBufSize);
    }
}

int TAbstractFront::sampleCount()
{
    if(mFilter)
        return mFilter->sampleCount();

    return 0;
}

void TAbstractFront::setAudioParameter(AudioParameter type, float value, int param)
{
    if(!mFilter)
        return;

    switch (type) {
        case AP_VOLUME:
            mFilter->setVolume(value);
            break;
        case AP_VOLUME_ENABLE:
            mAudioEnabled = (bool)value;
            break;
        case AP_EQUALIZER_ENABLE:
            mFilter->setEqualizerEnabled((bool)value);
            break;
        case AP_BALLANCE:
            mFilter->setBallance(value);
            break;
        case AP_EFFECT:
            mFilter->set3DEffectValue(value);
            break;
        case AP_AMPLIFICATION:
            mFilter->setAmplification(value);
            break;
        case AP_EQUALIZER_FACTOR:
            mFilter->setEqualizerFactor(param, value);
            break;
        case AP_EQUALIZER_RANGE:
            mFilter->setEqualizerFactor(param, value);
            break;
        case AP_RESET:
        default:
            mFilter->reset();
            if(mLoopBuf)
                mLoopBuf->reset();
            break;
    }
}

void TAbstractFront::getAudioData(AudioDataType dataType, void *param1, void *param2)
{
    if(!mFilter)
        return;

    switch (dataType) {
    case ADT_SAMPLE:
        *(short**)param1 = mSamplesBuf;
        *(int*)param2 = mSamplesCount;
        break;
    case ADT_SPECTRUM:
        mFilter->getSpectrumArray((TSpectrumElement**)param1, (int*)param2);
        break;
    case ADT_SILENT_FRAME:
        *(int*)param1 = mFilter->getSilentFrames();
        break;
    case ADT_SILENT_MICRO_SECONDS:
        *(int*)param1 = mFilter->getSilentFrames()*1000*mSamplesCount/SAMPLE_RATE;
        break;
    default:
        break;
    }
}

void TAbstractFront::read(byte *buf, int size)
{
    if(mBackendPlugin)
    {
        // size = 2 channels * sizeof(short) * samples
        mBackendPlugin->getNextSamples(buf, size);
    }
}
