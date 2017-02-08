#include "directsoundfront.h"

TDirectSoundFront::TDirectSoundFront() :
    TAbstractFront(),
    mPaused(false),
    mLoopLength(0),
    mNextSegment(0),
    mInitialized(false),
    mSamples(NULL),
    mDirectSound(NULL),
    mMainBuffer(NULL),
    mSecondBuffer(NULL)
{
    DSBUFFERDESC dsbuf;
    WAVEFORMATEX format;

    // Calculate the Seg Length and Loop length
    // (round to nearest sample)
    mSoundSegLength = (SAMPLE_RATE*10+(SOUND_FPS>>1))/SOUND_FPS;
    mLoopLength = (mSoundSegLength*SOUND_SEGMENTS)<<2;

    // create an IDirectSound COM object

    if (DS_OK != DirectSoundCreate(NULL, &mDirectSound, NULL))
    {
        printf("Unable to create DirectSound object!\n");
        return;
    }

    // set cooperative level where we need it

    if (DS_OK != IDirectSound_SetCooperativeLevel(mDirectSound, GetForegroundWindow(), DSSCL_PRIORITY))
    {
        printf("Unable to set cooperative level!\n");
        return;
    }

    // now create a primary sound buffer
    memset(&format, 0, sizeof(format));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.wBitsPerSample = 16;
    format.nSamplesPerSec = SAMPLE_RATE;
    format.nBlockAlign = 4;	// stereo 16-bit
    format.cbSize = 0;
    format.nAvgBytesPerSec=format.nSamplesPerSec*format.nBlockAlign;

    memset(&dsbuf, 0, sizeof(dsbuf));
    dsbuf.dwSize = sizeof(DSBUFFERDESC);
    dsbuf.dwFlags = DSBCAPS_PRIMARYBUFFER;
    dsbuf.dwBufferBytes = 0;
    dsbuf.lpwfxFormat = NULL;

    if (DS_OK != IDirectSound_CreateSoundBuffer(mDirectSound, &dsbuf, &mMainBuffer, NULL))
    {
        printf("Unable to create primary buffer!");
        return;
    }

    // Set it's format how we want
    if (DS_OK != IDirectSoundBuffer_SetFormat(mMainBuffer, &format))
    {
        printf("Unable to set primary buffer format!\n");
        return;
    }

    // Start the primary buffer playing now so we get
    // minimal lag when we trigger our secondary buffer

    IDirectSoundBuffer_Play(mMainBuffer, 0, 0, DSBPLAY_LOOPING);

    // that's done, now let's create our secondary buffer

    memset(&dsbuf, 0, sizeof(DSBUFFERDESC));
    dsbuf.dwSize = sizeof(DSBUFFERDESC);
    // we'll take default controls for this one
    dsbuf.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY;
    dsbuf.dwBufferBytes = mLoopLength;
    dsbuf.lpwfxFormat = (LPWAVEFORMATEX)&format;

    if (DS_OK != IDirectSound_CreateSoundBuffer(mDirectSound, &dsbuf, &mSecondBuffer, NULL))
    {
        printf("Unable to create secondary buffer\n");
        return;
    }

    // ok, cool, we're ready to go!
    // blank out the entire sound buffer
    {
        LPVOID ptr; DWORD len;

        IDirectSoundBuffer_Lock(mSecondBuffer, 0, 0, &ptr, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
        ZeroMemory(ptr, len);
        IDirectSoundBuffer_Unlock(mSecondBuffer, ptr, len, 0, 0);
    }

    // allocate and zero our local buffer
    mSamples = (INT16 *)malloc(mSoundSegLength<<2);
    ZeroMemory(mSamples, mSoundSegLength<<2);

    mInitialized = true;
}

TDirectSoundFront::~TDirectSoundFront()
{
    if (mSecondBuffer)
    {
        IDirectSoundBuffer_Stop(mSecondBuffer);
        IDirectSoundBuffer_Release(mSecondBuffer);
        mSecondBuffer = NULL;
    }

    if (mMainBuffer)
    {
        IDirectSoundBuffer_Stop(mMainBuffer);
        IDirectSoundBuffer_Release(mMainBuffer);
        mMainBuffer = NULL;
    }

    if (mDirectSound)
    {
        IDirectSound_Release(mDirectSound);
        mDirectSound = NULL;
    }

    if (mSamples)
    {
        free(mSamples);
    }
}

bool TDirectSoundFront::start(void)
{
    //waveLogStart();

    IDirectSound_SetCooperativeLevel(mDirectSound, GetForegroundWindow(), DSSCL_PRIORITY);

    IDirectSoundBuffer_SetCurrentPosition(mSecondBuffer, 0);
    IDirectSoundBuffer_Play(mSecondBuffer, 0, 0, DSBPLAY_LOOPING);

    return true;
}

void TDirectSoundFront::stop(void)
{
    DSBUFFERDESC	dsbuf;
    WAVEFORMATEX	format;

    //waveLogStop();

    IDirectSoundBuffer_Stop(mSecondBuffer);
    // this is a bit cheezity-hacky
    IDirectSoundBuffer_Release(mSecondBuffer);

    memset(&format, 0, sizeof(format));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.wBitsPerSample = 16;
    format.nSamplesPerSec = SAMPLE_RATE;
    format.nBlockAlign = 4;	// stereo 16-bit
    format.cbSize = 0;
    format.nAvgBytesPerSec=format.nSamplesPerSec*format.nBlockAlign;

        memset(&dsbuf, 0, sizeof(DSBUFFERDESC));
        dsbuf.dwSize = sizeof(DSBUFFERDESC);
    // we'll take default controls for this one
        dsbuf.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY;
        dsbuf.dwBufferBytes = mLoopLength;
    dsbuf.lpwfxFormat = (LPWAVEFORMATEX)&format;

    if (DS_OK != IDirectSound_CreateSoundBuffer(mDirectSound, &dsbuf, &mSecondBuffer, NULL))
    {
        printf("Unable to create secondary buffer\n");
        return;
    }

    // zero out the buffer
    {
        LPVOID ptr; DWORD len;

        IDirectSoundBuffer_Lock(mSecondBuffer, 0, 0, &ptr, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
        ZeroMemory(ptr, len);
        IDirectSoundBuffer_Unlock(mSecondBuffer, ptr, len, 0, 0);
    }
}

void TDirectSoundFront::flush(void)
{
    int oldpause = mPaused;

    mPaused = true;
    memset(mSamples, 0, mSoundSegLength * 4);

    for(int i=0;i<5;i++)
        step();

    mPaused = oldpause;
}

void TDirectSoundFront::step(void)
{
    int nPlaySeg=0, nFollowingSeg=0;
    DWORD nPlay=0, nWrite=0;
    int nRet=0;

    if (!mDirectSound)
        return;

    // We should do nothing until nPlay has left mNextSegment
    IDirectSoundBuffer_GetCurrentPosition(mSecondBuffer, &nPlay, &nWrite);

    nPlaySeg=nPlay/(mSoundSegLength<<2);

    if (nPlaySeg>SOUND_SEGMENTS-1) nPlaySeg=SOUND_SEGMENTS-1;
    if (nPlaySeg<0) nPlaySeg=0; // important to ensure nPlaySeg clipped for below

    if (mNextSegment == nPlaySeg)
    {
        Sleep(200); // Don't need to do anything for a bit
        return;
    }

    // work out which seg we will fill next
    nFollowingSeg = mNextSegment;
    WRAP_INC(nFollowingSeg)

    while (mNextSegment != nPlaySeg)
    {
        void *pData=NULL,*pData2=NULL; DWORD cbLen=0,cbLen2=0;

        // fill nNextSeg
        // Lock the relevant seg of the loop buffer
        nRet = IDirectSoundBuffer_Lock(mSecondBuffer, mNextSegment*(mSoundSegLength<<2), mSoundSegLength<<2, &pData, &cbLen, &pData2, &cbLen2, 0);

        if (nRet>=0 && pData!=NULL)
        {
          // Locked the seg okay - write the sound we calculated last time
            memcpy(pData, mSamples, mSoundSegLength<<2);
        }
        // Unlock (2nd 0 is because we wrote nothing to second part)
        if (nRet>=0) IDirectSoundBuffer_Unlock(mSecondBuffer, pData, cbLen, pData2, 0);

        // generate more samples
        if (!mPaused)
        {
            //printf("callback: %ld samples\n", samples);
            requestNextSamples(mSoundSegLength, mSamples);
        }
        else
        {
            memset(mSamples, 0, mSoundSegLength*4);
        }

        //waveLogFrame((unsigned char *)mSamples, mSoundSegLength<<2);

        mNextSegment = nFollowingSeg;
        WRAP_INC(nFollowingSeg)
    }

    return;
}

void TDirectSoundFront::play()
{

}

void TDirectSoundFront::pause()
{

}

void TDirectSoundFront::setRecorder()
{

}

void TDirectSoundFront::setFilter()
{

}

void TDirectSoundFront::setSampleSize(int sampleSize)
{
    (void)sampleSize;
}
