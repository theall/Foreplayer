#include "sdlfront.h"

// Sound output driver using SDL
#include <SDL2/SDL.h>
#undef main

static void sdl_callback( void* data, Uint8* out, int count )
{
    TSDLFront *front = static_cast<TSDLFront*>(data);
    if(front)
    {
        SDL_memset(out, 0, count);
        front->requestNextSamples(count/2, (short*)out);
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

bool TSDLFront::start()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

    int min_size = SAMPLE_RATE * 2 / SOUND_FPS;
    int buf_size = 512;
    while ( buf_size < min_size )
        buf_size *= 2;

    static SDL_AudioSpec as; // making static clears all fields to 0
    as.freq     = SAMPLE_RATE;
    as.format   = AUDIO_S16SYS;
    as.channels = 2;
    as.callback = sdl_callback;
    as.samples  = buf_size;
    as.userdata = this;
    if (SDL_OpenAudio(&as, 0) < 0)
    {
        const char* err = SDL_GetError();
        if ( !err )
            qDebug() << "Couldn't open SDL audio," << err;
    }

    return true;
}

void TSDLFront::stop()
{
    SDL_PauseAudio(true);

    // be sure audio thread is not active
    SDL_LockAudio();
    SDL_UnlockAudio();
}

void TSDLFront::step()
{

}

void TSDLFront::play()
{
    SDL_PauseAudio(false);
}

void TSDLFront::pause()
{
    stop();
}
