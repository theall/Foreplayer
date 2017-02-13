
#include "samplesfilter.h"

#include <math.h>
#include <memory.h>

#define SAMPLE_AMPLITUDE_MIN        -32767
#define SAMPLE_AMPLITUDE_MAX        32768
#define SAMPLE_SILENT_CHECK_VALUE   16
#define PI                          3.14159265358979323846
// Fudge factor used to calculate the spectrum bar heights
const float SpectrumAnalyserMultiplier = 0.15;

struct TEqualizerRange
{
    int min;
    int max;
    float factor;
};

TEqualizerRange g_eqTable[SPECTRUM_COUNT] = {
    { 20, 40, 1.0f},
    { 40, 80, 1.0f},
    { 80, 160, 1.0f},
    { 160, 320, 1.0f},
    { 320, 640, 1.0f},
    { 640, 1280, 1.0f},
    { 1280, 2560, 1.0f},
    { 2560, 5120, 1.0f},
    { 5120, 10240, 1.0f},
    { 10240, 20480, 1.0f}
};

TSamplesFilter::TSamplesFilter(int sampleRate) :
    mVolume(1.0)
  , mBallanceL(1.0)
  , mBallanceR(1.0)
  , mAmplification(0.0)
  , mEffectValue(1.0)
  , mSampleRate(sampleRate)
  , mSampleCount(1<<FFTLengthPowerOfTwo)
  , mSilentFrames(0)
  , mWindowFunction(NoWindow)
  , mFFT(new FFTRealWrapper)
{   
    mSampleBufSize = mSampleCount * sizeof(FFTRealWrapper::DataType);
    mTimeDomainBuf = (FFTRealWrapper::DataType*)malloc(mSampleBufSize);
    mFreqDomainBuf = (FFTRealWrapper::DataType*)malloc(mSampleBufSize);

    mSpectrumArraySize = mSampleCount / 2;
    mSpectrumArray = (TSpectrumElement*)malloc(mSpectrumArraySize*sizeof(TSpectrumElement));
    initWindow();
}

TSamplesFilter::~TSamplesFilter()
{
    if(mTimeDomainBuf)
    {
        delete mTimeDomainBuf;
        mTimeDomainBuf = NULL;
    }
    if(mFreqDomainBuf)
    {
        delete mFreqDomainBuf;
        mFreqDomainBuf = NULL;
    }
    if(mWindow)
    {
        delete mWindow;
        mWindow = NULL;
    }
    if(mSpectrumArray)
    {
        delete mSpectrumArray;
        mSpectrumArray = NULL;
    }
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
    int sumFrame = 0;
    memset(mTimeDomainBuf, 0, mSampleBufSize);
    for (int i=0; i<dwSamples; i++) {
        const short pcmSample = *ptr;
        sumFrame += (unsigned short)pcmSample;
        mTimeDomainBuf[i] = pcmSample;
        ptr += 2;
    }

    if(sumFrame==0)
        mSilentFrames++;
    else
        mSilentFrames = 0;

    // To frequency domain
    mFFT->calculateFFT(mTimeDomainBuf, mFreqDomainBuf);

    int halfSampleCount = mSampleCount/2;
    // Analyze output to obtain amplitude and phase for each frequency
    for (int i=0; i<=halfSampleCount; ++i) {
        // Calculate frequency of this complex sample
        float freq = float(i * mSampleRate) / (mSampleCount);
        float real = mFreqDomainBuf[i];
        float imag = 0.0;
        if (i>0 && i<halfSampleCount)
            imag = mFreqDomainBuf[halfSampleCount + i];

        // Apply amplitude of spectrums
        for(int j=0;j<SPECTRUM_COUNT;j++)
        {
            TEqualizerRange *eqRange = &g_eqTable[j];
            if(eqRange->factor!=1.0f && freq>=eqRange->min && freq<eqRange->max)
            {
                real *= eqRange->factor*0.75;
                imag *= eqRange->factor*0.25;

                mFreqDomainBuf[i] = real;
                mFreqDomainBuf[halfSampleCount + i] = imag;
            }
        }
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

    // now apply the post volume
    if (mVolume != 1.0f)
    {
        for (int i = 0; i < dwSamples; i++)
        {
            mTimeDomainBuf[i] = mTimeDomainBuf[i] * mVolume;
        }
    }

    if(mAmplification != 0.0f)
    {
        for (int i = 0; i < dwSamples; i++)
        {
            mTimeDomainBuf[i] = mTimeDomainBuf[i] * mAmplification;
        }
    }

    // Ballance
    int index = 0;
    for (int i = 0; i < dwSamples; i++)
    {
        int l = mTimeDomainBuf[i] * mBallanceL;
        int r = mTimeDomainBuf[i] * mBallanceR;
        if(l <= SAMPLE_AMPLITUDE_MIN || l>=SAMPLE_AMPLITUDE_MAX)
            l = 0;
        if(r <= SAMPLE_AMPLITUDE_MIN || l>=SAMPLE_AMPLITUDE_MAX)
            r = 0;

        out[index++] = (short)l;
        out[index++] = (short)r;
    }

    // To frequency domain
    mFFT->calculateFFT(mTimeDomainBuf, mFreqDomainBuf);

    // Analyze output to obtain amplitude and phase for each frequency
    for (int i=0; i<=halfSampleCount; ++i) {
        // Calculate frequency of this complex sample
        float freq = float(i * mSampleRate) / (mSampleCount);

        mSpectrumArray[i].frequency = freq;

        float real = mFreqDomainBuf[i];
        float imag = 0.0;
        if (i>0 && i<halfSampleCount)
            imag = mFreqDomainBuf[halfSampleCount + i];

        const float magnitude = sqrt(real*real + imag*imag);
        float amplitude = 0.25 * magnitude / SAMPLE_AMPLITUDE_MAX;

        // Bound amplitude to [0.0, 1.0]
        mSpectrumArray[i].clipped = (amplitude > 1.0);
        amplitude = std::max(float(0.0), amplitude);
        amplitude = std::min(float(1.0), amplitude);
        mSpectrumArray[i].amplitude = amplitude;
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
    mEffectValue = value;
}

void TSamplesFilter::setSpectrumFactor(int index, float value)
{
    if(index<0 || index>=SPECTRUM_COUNT)
        return;

    g_eqTable[index].factor = pow(10, value/20);
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
