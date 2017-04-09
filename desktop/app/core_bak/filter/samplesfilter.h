#ifndef TSAMPLESFILTER_H
#define TSAMPLESFILTER_H

#include <mutex>
#include <fftreal/fftreal_wrapper.h>
#include <audio3d/audio_3d.h>

#define EQUALIZER_SEGMENTS  10

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
    std::mutex mMutex;

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
