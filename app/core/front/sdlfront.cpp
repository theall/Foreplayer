#include "sdlfront.h"

// Sound output driver using SDL
#include <SDL2/SDL.h>
#undef main

#define DEFAULT_SAMPLE_SIZE 512
//static int nSamples = 0;
//#include <QDebug>
static void sdl_callback( void* data, Uint8* out, int count )
{
    TSDLFront *front = static_cast<TSDLFront*>(data);
    if(front && front->isPlaying())
    {
        SDL_memset(out, 0, count);
        front->requestNextSamples(count/2, (short*)out);

//        nSamples++;
//        if(nSamples%60==0)
//            qDebug() << nSamples;
    }
}

TSDLFront::TSDLFront()
{

}

TSDLFront::~TSDLFront()
{
    stop();

    SDL_CloseAudio();
    SDL_Quit();
}

void TSDLFront::setRecorder()
{

}

void TSDLFront::setFilter()
{

}

void TSDLFront::setSampleSize(int sampleSize)
{
    if(mSamplesSize == sampleSize)
        return;

    SDL_CloseAudio();

    mSamplesSize = sampleSize;

    if(mSamplesSize <= 0)
    {
        int min_size = SAMPLE_RATE * 2 / SOUND_FPS;
        int buf_size = DEFAULT_SAMPLE_SIZE;
        while ( buf_size < min_size )
            buf_size *= 2;
        mSamplesSize = buf_size;
    }

    static SDL_AudioSpec as; // making static clears all fields to 0
    as.freq     = SAMPLE_RATE;
    as.format   = AUDIO_S16SYS;
    as.channels = 2;
    as.callback = sdl_callback;
    as.samples  = mSamplesSize;
    as.userdata = this;
    if (SDL_OpenAudio(&as, 0) < 0)
    {
        const char* err = SDL_GetError();
        if ( !err )
            qDebug() << "Couldn't open SDL audio," << err;
    }
}

bool TSDLFront::start()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

    setSampleSize(DEFAULT_SAMPLE_SIZE);

    return true;
}

void TSDLFront::stop()
{
    mPlaying = false;
    // be sure audio thread is not active
    SDL_LockAudio();

    SDL_PauseAudio(true);

    SDL_UnlockAudio();
}

void TSDLFront::step()
{

}

void TSDLFront::play()
{
    SDL_PauseAudio(false);

    // be sure audio thread is not active
    SDL_LockAudio();
    SDL_UnlockAudio();
    mPlaying = true;
}

void TSDLFront::pause()
{
    stop();
}
