#ifndef TSAMPLESFILTER_H
#define TSAMPLESFILTER_H

#include <mutex>
#include <fftreal/fftreal_wrapper.h>

#define SPECTRUM_COUNT  10

enum WindowFunction {
    NoWindow,
    HannWindow
};

struct TSpectrumElement {
    TSpectrumElement()
    :   frequency(0.0), amplitude(0.0), phase(0.0), clipped(false)
    { }

    /**
     * Frequency in Hertz
     */
    qreal frequency;

    /**
     * Amplitude in range [0.0, 1.0]
     */
    qreal amplitude;

    /**
     * Phase in range [0.0, 2*PI]
     */
    qreal phase;

    /**
     * Indicates whether value has been clipped during spectrum analysis
     */
    bool clipped;
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
    void setSpectrumFactor(int index, float value);

    void getSpectrumArray(TSpectrumElement **spectrumArray, int *size);
    int getSilentFrames();
    int sampleCount() { return mSampleCount; }

private:
    float mHeadMix;
    float mVolume;
    float mBallanceL;
    float mBallanceR;
    float mAmplification;
    float mEffectValue;
    int mSampleRate;
    std::mutex mMutex;

    int mSampleCount;
    int mSampleBufSize;
    int mSilentFrames;
    FFTRealWrapper::DataType *mTimeDomainBuf;
    FFTRealWrapper::DataType *mFreqDomainBuf;
    FFTRealWrapper::DataType *mWindow;
    WindowFunction mWindowFunction;
    FFTRealWrapper* mFFT;
    TSpectrumElement *mSpectrumArray;

    void initWindow();
    float valueFactor(int value);
};

#endif
