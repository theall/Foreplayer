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
#ifndef TSAMPLESFILTER_H
#define TSAMPLESFILTER_H

#include <foreplayer.h>
#include <fftreal/fftreal_wrapper.h>
#include <audio3d/include/audio_3d.h>

#define EQUALIZER_SEGMENTS  10

using namespace std;

enum WindowFunction {
    NoWindow,
    HannWindow
};

class TSamplesFilter
{
public:
    TSamplesFilter(int sampleRate = 44100);
    ~TSamplesFilter();

    void filter(int dwSamples, short *out);
    void setVolume(float value);
    void setVolume(int value);
    float volume();

    void setBallance(float value);
    void setBallance(float left, float right);
    void setAmplification(float value);
    void set3DEffectValue(float value);
    void setEqualizerFactor(int index, float value);
    void setEqualizerRange(int index, int min, int max);
    void setEqualizerEnabled(bool enabled);

    void getSpectrumArray(TSpectrumElement **spectrumArray, int *size);
    int getSilentFrames();

    void reset();
    int sampleCount() { return mSampleCount; }

private:
    float mHeadMix;
    float mVolume;
    float mBallanceL;
    float mBallanceR;
    float mAmplification;
    float m3dEffectValue;
    int mSampleRate;

    int mSampleCount;
    int mSampleBufSize;
    int mSilentFrames;
    bool mEqualizerEnabled;
    FFTRealWrapper::DataType *mTimeDomainBuf;
    FFTRealWrapper::DataType *mFreqDomainBuf;
    FFTRealWrapper::DataType *mWindow;
    WindowFunction mWindowFunction;
    FFTRealWrapper* mFFT;

    int mSpectrumArraySize;
    TSpectrumElement *mSpectrumArray;

    int mElevation;
    int mAzimuth;
    int mDistance;
    Audio3DSource *mAudioSource;
    void initWindow();
    float valueFactor(int value);
};

#endif
