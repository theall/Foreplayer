#ifndef TABSTRACTFRONT
#define TABSTRACTFRONT

#include "../plugins/backend/backendinterface.h"

#define SAMPLE_RATE         44100
#define SOUND_SEGMENTS      48
#define SOUND_FPS           600

class TAbstractFront
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
    virtual void setSampleSize(int sampleSize) = 0;

    void setCallback(TRequestSamples callback);
    void requestNextSamples(int size, short *samples);
    void currentSamples(int *size, short **samples);
    inline bool isPlaying() { return mPlaying; }

protected:
    int mSamplesSize;
    short *mSamples;
    bool mPlaying;

private:
    TRequestSamples mCallback;
};

#endif // TABSTRACTFRONT
