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

#include "samplesfilter.h"

#include <math.h>
#include <memory.h>

#define SAMPLE_AMPLITUDE_MIN        -32767
#define SAMPLE_AMPLITUDE_MAX        32768
#define SAMPLE_AMPLITUDE_CLIP       32000
#define SAMPLE_SILENT_CHECK_VALUE   16
#define PI                          3.14159265358979323846

#define DELETE_POINTER(x) \
if(x) \
{ \
    delete x;\
    x = NULL;\
}

// Fudge factor used to calculate the spectrum bar heights
const float SpectrumAnalyserMultiplier = 0.15;

struct TEqualizerRange
{
    int min;
    int max;
    float factor;
};

TEqualizerRange g_eqTable[EQUALIZER_SEGMENTS] = {
    { 20, 80, 1.0f},
    { 80, 200, 1.0f},
    { 200, 250, 1.0f},
    { 250, 400, 1.0f},
    { 400, 800, 1.0f},
    { 800, 1228, 1.0f},
    { 1228, 2560, 1.0f},
    { 2560, 4096, 1.0f},
    { 4096, 6553, 1.0f},
    { 6553, 16384, 1.0f}
    //{ 16384, 20480, 1.0f}
};

TSamplesFilter::TSamplesFilter(int sampleRate) :
    mVolume(1.0)
  , mBallanceL(1.0)
  , mBallanceR(1.0)
  , mAmplification(0.0)
  , m3dEffectValue(0.0)
  , mSampleRate(sampleRate)
  , mSampleCount(1<<FFTLengthPowerOfTwo)
  , mSilentFrames(0)
  , mEqualizerEnabled(true)
  , mWindowFunction(NoWindow)
  , mFFT(new FFTRealWrapper)
  , mElevation(0)
  , mAzimuth(0)
  , mDistance(0)
{
    mSampleBufSize = mSampleCount * sizeof(FFTRealWrapper::DataType);
    mTimeDomainBuf = (FFTRealWrapper::DataType*)malloc(mSampleBufSize);
    mFreqDomainBuf = (FFTRealWrapper::DataType*)malloc(mSampleBufSize);

    mSpectrumArraySize = mSampleCount / 2;
    mSpectrumArray = (TSpectrumElement*)malloc(mSpectrumArraySize*sizeof(TSpectrumElement));

    mAudioSource = new Audio3DSource(sampleRate, mSampleCount);
    mAudioSource->SetDirection(mElevation, mAzimuth, mDistance);

    initWindow();
}

TSamplesFilter::~TSamplesFilter()
{
    DELETE_POINTER(mTimeDomainBuf);
    DELETE_POINTER(mFreqDomainBuf);
    DELETE_POINTER(mWindow);
    DELETE_POINTER(mSpectrumArray);
    DELETE_POINTER(mAudioSource);
}

float TSamplesFilter::valueFactor(int value)
{
    float vf = 1.0;
    if(value < 0)
        vf = 0.0;
    else if(value<100)
        vf = (float)value / 100;

    return vf;
}

void TSamplesFilter::filter(int dwSamples, short *out)
{
    if(dwSamples > mSampleCount)
        return;

    mMutex.lock();
    short *ptr = out;

    // Fill time domain buf with one channel samples
    memset(mTimeDomainBuf, 0, mSampleBufSize);
    int sumFrame = 0;
    for (int i=0; i<dwSamples; i++) {
        const short pcmSample = *ptr;
        sumFrame += (unsigned short)pcmSample;
        mTimeDomainBuf[i] = pcmSample;
        ptr += 2;
    }

    if(sumFrame)
        mSilentFrames = 0;
    else
        mSilentFrames++;

    // apply the post volume
    if (mVolume != 1.0f)
    {
        for (int i = 0; i < dwSamples; i++)
        {
            mTimeDomainBuf[i] = mTimeDomainBuf[i] * mVolume;
        }
    }

    // Amplitude scale
    if(mEqualizerEnabled && mAmplification != 0.0f)
    {
        for (int i = 0; i < dwSamples; i++)
        {
            mTimeDomainBuf[i] = mTimeDomainBuf[i] * mAmplification;
        }
    }

    // To frequency domain to apply equalizer parameters
    mFFT->calculateFFT(mTimeDomainBuf, mFreqDomainBuf);
    int halfSampleCount = mSampleCount/2;
    // Analyze output to obtain amplitude and phase for each frequency
    for (int i=0; i<=halfSampleCount; ++i) {
        // Calculate frequency of this complex sample
        float freq = float(i * mSampleRate) / (mSampleCount);
        float real = mFreqDomainBuf[i];
        float image = 0.0;
        if (i>0 && i<halfSampleCount)
            image = mFreqDomainBuf[halfSampleCount + i];

        if(mEqualizerEnabled)
        {
            // Apply amplitude of spectrums
            for(int j=0;j<EQUALIZER_SEGMENTS;j++)
            {
                TEqualizerRange *eqRange = &g_eqTable[j];
                if(eqRange->factor!=1.0f && freq>=eqRange->min && freq<eqRange->max)
                {
                    real *= eqRange->factor*0.5;
                    image *= eqRange->factor*0.5;
                    mFreqDomainBuf[i] = real;
                    mFreqDomainBuf[halfSampleCount + i] = image;
                    break;
                }
            }
        }

        // Fill spectrum arrays
        mSpectrumArray[i].frequency = freq;
        const float magnitude = sqrt(real*real + image*image);
        // Bound amplitude to [0.0, 1.0]
        float amplitude = 0.25 * magnitude / SAMPLE_AMPLITUDE_MAX;
        mSpectrumArray[i].clipped = (amplitude > 1.0);
        amplitude = max(float(0.0), amplitude);
        amplitude = min(float(1.0), amplitude);
        mSpectrumArray[i].amplitude = amplitude;
    }

    // To time domain
    memset(mTimeDomainBuf, 0, mSampleBufSize);
    mFFT->calculateIFFT(mFreqDomainBuf, mTimeDomainBuf);
    for (int i = 0; i < dwSamples; i++)
    {
        FFTRealWrapper::DataType temp = mTimeDomainBuf[i];
        if(temp > 0)
            mTimeDomainBuf[i] = int(temp/mSampleCount+0.5);
        else
            mTimeDomainBuf[i] = int(temp/mSampleCount-0.5);
    }

    float *outputLeft = mTimeDomainBuf;
    float *outputRight = mTimeDomainBuf;

    // 3d effect
    if(m3dEffectValue > 0)
    {
        vector<float> input;
        vector<float> outL;
        vector<float> outR;
        for(int i=0;i<mSampleCount;i++)
            input.push_back(mTimeDomainBuf[i]);

        mAudioSource->ProcessBlock(input, &outL, &outR);

        outputRight = mFreqDomainBuf;
        for(int i=0;i<mSampleCount;i++)
        {
            *outputLeft++ = outL[i];
            *outputRight++ = outR[i];
        }
        outputLeft = mTimeDomainBuf;
        outputRight = mFreqDomainBuf;
    }

    // Ballance
    int index = 0;
    for (int i = 0; i < dwSamples; i++)
    {
        int v = outputLeft[i] * mBallanceL;
        if(v > SAMPLE_AMPLITUDE_CLIP)
            v = SAMPLE_AMPLITUDE_CLIP;
        else if(v < -SAMPLE_AMPLITUDE_CLIP)
            v = -SAMPLE_AMPLITUDE_CLIP;

        out[index++] = v;
        v = outputRight[i] * mBallanceR;
        if(v > SAMPLE_AMPLITUDE_CLIP)
            v = SAMPLE_AMPLITUDE_CLIP;
        else if(v < -SAMPLE_AMPLITUDE_CLIP)
            v = -SAMPLE_AMPLITUDE_CLIP;

        out[index++] = v;
    }

    mMutex.unlock();
}

void TSamplesFilter::setVolume(float value)
{
    mMutex.lock();
    mVolume = value;
    mMutex.unlock();
}

void TSamplesFilter::setVolume(int value)
{
    setVolume(valueFactor(value));
}

float TSamplesFilter::volume()
{
    return mVolume;
}

void TSamplesFilter::setBallance(float value)
{
    mMutex.lock();
    if(value > 1.0)
        value = 1.0;
    else if(value < 0.0f)
        value = 0.0;

    mBallanceL = value>0.5?(1-value)*2:1.0;
    mBallanceR = value<0.5?value*2:1.0;
    mMutex.unlock();
}

void TSamplesFilter::setBallance(float left, float right)
{
    mMutex.lock();
    mBallanceL = left;
    mBallanceR = right;
    mMutex.unlock();
}

void TSamplesFilter::setAmplification(float value)
{
    mAmplification = pow(10, value/10);
}

void TSamplesFilter::set3DEffectValue(float value)
{
    m3dEffectValue = value;
}

void TSamplesFilter::setEqualizerFactor(int index, float value)
{
    if(index<0 || index>=EQUALIZER_SEGMENTS)
        return;

    g_eqTable[index].factor = pow(10, value/20);
}

void TSamplesFilter::setEqualizerRange(int index, int min, int max)
{
    if(index<0 || index>=EQUALIZER_SEGMENTS)
        return;

    g_eqTable[index].min = min;
    g_eqTable[index].max = max;
}

void TSamplesFilter::setEqualizerEnabled(bool enabled)
{
    mEqualizerEnabled = enabled;
}

void TSamplesFilter::getSpectrumArray(TSpectrumElement **spectrumArray, int *size)
{
    *spectrumArray = mSpectrumArray;
    *size = mSpectrumArraySize;
}

int TSamplesFilter::getSilentFrames()
{
    return mSilentFrames;
}

void TSamplesFilter::reset()
{
    mSilentFrames = 0;
}

void TSamplesFilter::initWindow()
{
    mWindow = (FFTRealWrapper::DataType*)malloc(mSampleBufSize);
    for (int i=0; i<mSampleCount; ++i) {
        FFTRealWrapper::DataType x = 0.0;

        switch (mWindowFunction) {
        case HannWindow:
            x = 0.5 * (1 - cos((2 * PI * i) / (mSampleCount - 1)));
            break;
        case NoWindow:
        default:
            x = 1.0;
            break;
        }
        mWindow[i] = x;
    }
}
