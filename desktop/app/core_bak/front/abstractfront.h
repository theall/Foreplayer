#ifndef TABSTRACTFRONT
#define TABSTRACTFRONT

#include "../filter/samplesfilter.h"
#include "../plugins/backend/backendinterface.h"

#include "loopbuffer.h"

#define SAMPLE_RATE         44100
#define SOUND_SEGMENTS      48
#define SOUND_FPS           600

enum TAudioParameter
{
    AP_VOLUME,
    AP_VOLUME_ENABLE,
    AP_BALLANCE,
    AP_EFFECT,
    AP_AMPLIFICATION,
    AP_EQUALIZER_ENABLE,
    AP_EQUALIZER_FACTOR,
    AP_EQUALIZER_RANGE,
    AP_RESET
};

enum TAudioDataType
{
    ADT_SAMPLE,
    ADT_SPECTRUM,
    ADT_SILENT_FRAME,
    ADT_SILENT_MICRO_SECONDS
};

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

    void setCallback(TRequestSamples callback);
    void requestNextSamples(int bufSize, char *samples);

    bool isPlaying() { return mPlaying; }
    int sampleCount();

    void setAudioParameter(TAudioParameter type, float value, int param);
    void getAudioData(TAudioDataType dataType, void *param1, void* param2);

protected:
    bool mAudioEnabled;
    int mSamplesCount;
    short *mSamplesBuf;
    bool mPlaying;

private:
    TSamplesFilter *mFilter;
    TRequestSamples mCallback;
    TLoopBuffer *mLoopBuf;

    // IDataCallback interface
private:
    void read(byte *buf, int size) override;
};

#endif // TABSTRACTFRONT
