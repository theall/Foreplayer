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
#include "sdlfront.h"

// Sound output driver using SDL
#undef main
#include <SDL2/SDL.h>
#include <iostream>

#define DEFAULT_SAMPLE_SIZE 512

static void sdl_callback( void* data, Uint8* out, int count )
{
    SDL_memset(out, 0, count);
    TSDLFront *front = static_cast<TSDLFront*>(data);
    if(front && front->isPlaying())
    {
        front->requestNextSamples(count, (char*)out);
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
    if(sampleSize < 1)
        sampleSize = mSamplesCount*2;

    TAbstractFront::setSampleSize(sampleSize);
}

bool TSDLFront::start()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

    SDL_CloseAudio();

    int min_size = SAMPLE_RATE * 2 / SOUND_FPS;
    int buf_size = DEFAULT_SAMPLE_SIZE;
    while ( buf_size < min_size )
        buf_size *= 2;

    mSamplesCount = sampleCount();

    static SDL_AudioSpec as; // making static clears all fields to 0
    as.freq     = SAMPLE_RATE;
    as.format   = AUDIO_S16SYS;
    as.channels = 2;
    as.callback = sdl_callback;
    as.samples  = mSamplesCount;
    as.userdata = this;
    if (SDL_OpenAudio(&as, 0) < 0)
    {
        const char* err = SDL_GetError();
        if ( !err )
            cout << "Couldn't open SDL audio," << err;
    }

    return true;
}

void TSDLFront::stop()
{
    mPlaying = false;
    // be sure audio thread is not active
    SDL_LockAudio();

    SDL_PauseAudio(true);

    SDL_UnlockAudio();

    setAudioParameter(AP_RESET, 0, 0);
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
