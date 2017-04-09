/*
	dsnd.c - Win32 DirectSound output driver.

	Copyright (c) 1997-2001 R.B.
	All Rights Reserved.
*/

#include <math.h>
#include <stdio.h>
#include <dsound.h>

#include <SDL2/SDL.h>

#include "cpuintrf.h"
#include "oss.h"
#include "wavelog.h"

static INT16 *samples;	// make sure we reserve enough for worst-case scenario
static int s32SoundEnable=1;

void (*m1sdr_Callback)(unsigned long dwSamples, short *samples);
unsigned long cbUserData;

static int hw_present, oss_pause;

int nDSoundSamRate=44100;        // sample rate
int nDSoundSegCount=48;    // Segs in the pdsbLoop buffer
static int cbLoopLen=0;          // Loop length (in bytes) calculated

int nDSoundFps=600;              // Application fps * 10
int nDSoundSegLen=0;             // Seg length in samples (calculated from Rate/Fps)
short *DSoundNextSound=NULL;     // The next sound seg we will add to the sample loop
unsigned char bDSoundOkay=0;     // True if DSound was initted okay
unsigned char bDSoundPlaying=0;  // True if the Loop buffer is playing

#define WRAP_INC(x) { x++; if (x>=nDSoundSegCount) x=0; }

static void sdl_callback( void* data, Uint8* out, int count )
{
    UNUSED(data);

    SDL_memset(out, 0, count);
    if ((m1sdr_Callback) && (!oss_pause))
    {
        //printf("callback: %ld samples\n", samples);
        m1sdr_Callback(count/4, (signed short*)out);
    }
}

void m1sdr_TimeCheck(void)
{
	return;
}

// m1sdr_Init - sets up directsound how we want
//			  thanks to Michael Abrash in DDJ
//			  for ideas on this.

INT16 m1sdr_Init(int sample_rate, int useInternal)
{
    if (!s32SoundEnable)
        return(0);

	nDSoundSamRate = sample_rate;

	samples = NULL;

	// Calculate the Seg Length and Loop length
	// (round to nearest sample)
	nDSoundSegLen=(nDSoundSamRate*10+(nDSoundFps>>1))/nDSoundFps;
	cbLoopLen=(nDSoundSegLen*nDSoundSegCount)<<2;

    if(useInternal)
    {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
            return false;

        static SDL_AudioSpec as; // making static clears all fields to 0
        as.freq     = nDSoundSamRate;
        as.format   = AUDIO_S16SYS;
        as.channels = 2;
        as.callback = sdl_callback;
        as.samples  = nDSoundSegLen;
        as.userdata = 0;
        if (SDL_OpenAudio(&as, 0) < 0)
        {
            const char* err = SDL_GetError();
            if ( !err )
                printf("Couldn't open SDL audio,%s", err);
        }
    }
	// allocate and zero our local buffer
	samples = (INT16 *)malloc(nDSoundSegLen<<2);
    memset(samples, 0, nDSoundSegLen<<2);

	bDSoundOkay=1; // This module was initted okay

	return(1);
}

void m1sdr_Exit(void)
{
	if (!s32SoundEnable)
	{
		return;
	}
    SDL_CloseAudio();
	if (samples)
	{
		free(samples);
	}
}



void m1sdr_PlayStart(void)
{
	waveLogStart();

    SDL_LockAudio();
    SDL_PauseAudio(false);
    SDL_UnlockAudio();
}

void m1sdr_PlayStop(void)
{
	waveLogStop();
    SDL_LockAudio();
    SDL_PauseAudio(true);
    SDL_UnlockAudio();
}


INT32 m1sdr_HwPresent(void)
{
	return hw_present;
}

INT16 m1sdr_IsThere(void)
{
    return 0;
}

void m1sdr_SetCallback(void *fn)
{
	if (fn == (void *)NULL)
	{
		printf("ERROR: NULL CALLBACK!\n");
	}

//	printf("m1sdr_SetCallback: aok!\n");
	m1sdr_Callback = (void (*)(unsigned long, signed short *))fn;
}

void m1sdr_FlushAudio(void)
{
	int oldpause = oss_pause;

	oss_pause = 1;
	memset(samples, 0, nDSoundSegLen * 4);

    for(int i=0;i<5;i++)
        m1sdr_TimeCheck();

	oss_pause = oldpause;
}

void m1sdr_Pause(int set)
{
	oss_pause = set;
}

void m1sdr_SetNoWait(int nw)
{
    UNUSED(nw);
}

