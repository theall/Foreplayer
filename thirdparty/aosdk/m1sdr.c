/*
 * Audio Overload SDK
 *
 * Implementations of functions shared by all audio backends
 */

#include <stdio.h>
#include "ao.h"
#include "m1sdr.h"

#ifdef ENABLE_PLAY
#include <SDL2/SDL.h>
#endif

m1sdr_callback_t *m1sdr_Callback;
ao_bool hw_present;

void m1sdr_SetCallback(m1sdr_callback_t *function)
{
	if (function == NULL)
	{
		printf("ERROR: NULL CALLBACK!\n");
	}

	m1sdr_Callback = function;
}

ao_bool m1sdr_HwPresent(void)
{
	return hw_present;
}

#ifdef ENABLE_PLAY
int nDSoundFps = 600;
int nDSoundSamRate = 44100;
void m1sdr_SetSamplesPerTick(UINT32 spf)
{
    if (spf != (nDSoundFps/10))
    {
        m1sdr_Exit();
        m1sdr_Init( nDSoundSamRate);
    }
}

m1sdr_ret_t m1sdr_TimeCheck(void)
{
    return M1SDR_OK;
}

INT16 m1sdr_Init(int sample_rate)
{
    (void)sample_rate;
    static SDL_AudioSpec as; // making static clears all fields to 0
    as.freq     = sample_rate;
    as.format   = AUDIO_S16SYS;
    as.channels = 2;
    as.callback = sdl_callback;
    as.samples  = sample_rate/60/2;
    as.userdata = &audio_3d;
    SDL_OpenAudio(&as, 0);
    return(1);
}

void m1sdr_Exit(void)
{
    #ifdef ENABLE_PLAY

    #endif
}

void m1sdr_PlayStart(void)
{

}

void m1sdr_PlayStop(void)
{

}

void m1sdr_FlushAudio(void)
{
    //memset(samples, 0, nDSoundSegLen * 4);

    int i;
    for(i = 0;i<5;i++)
        m1sdr_TimeCheck();
}
#endif
