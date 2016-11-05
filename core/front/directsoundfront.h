#ifndef TDIRECTSOUNDFRONT_H
#define TDIRECTSOUNDFRONT_H

#include <windows.h>
#include <windowsx.h>
#include <cguid.h>
#include <math.h>
#include <stdio.h>
#include <dsound.h>

#include "abstractfront.h"

#define SAMPLE_RATE 44100
#define SOUND_SEGMENTS 48
#define SOUND_FPS 600

class TDirectSoundFront : public TAbstractFront
{
public:
    TDirectSoundFront();
    ~TDirectSoundFront();

    void start();
    void stop();

    void step();

    void setRecorder();
    void setFilter();

private:
    int mPaused;
    int mSoundSegLength;    // Seg length in samples (calculated from Rate/Fps)
    int mLoopLength;        // Loop length (in bytes) calculated
    int mNextSegment;       // We have filled the sound in the loop up to the beginning of 'nNextSeg'
    bool mInitialized;
    INT16 *mSamples;        // make sure we reserve enough for worst-case scenario
    LPDIRECTSOUND mDirectSound;			// DirectSound COM object
    LPDIRECTSOUNDBUFFER mMainBuffer;	// Primary DirectSound buffer
    LPDIRECTSOUNDBUFFER mSecondBuffer;	// Secondary DirectSound buffer
    void flush();
};

#define WRAP_INC(x) { x++; if (x>=mSoundSegLength) x=0; }

#endif // TDIRECTSOUNDFRONT_H
