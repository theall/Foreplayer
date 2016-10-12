// Sound output driver using SDL

#include "audiodriver.h"

static sound_callback_t sound_callback;
static void* sound_callback_data;

static void sdl_callback( void* data, Uint8* out, int count )
{
	if ( sound_callback )
		sound_callback( sound_callback_data, (short*) out, count / 2 );
}

static const char* sound_init( long sample_rate, int buf_size,
		sound_callback_t cb, void* data )
{
	sound_callback = cb;
	sound_callback_data = data;
	
	static SDL_AudioSpec as; // making static clears all fields to 0
	as.freq     = sample_rate;
	as.format   = AUDIO_S16SYS;
	as.channels = 2;
	as.callback = sdl_callback;
	as.samples  = buf_size;
	if ( SDL_OpenAudio( &as, 0 ) < 0 )
	{
		const char* err = SDL_GetError();
		if ( !err )
			err = "Couldn't open SDL audio";
		return err;
	}
	
	return 0;
}

static void sound_start()
{
	SDL_PauseAudio( false );
}

static void sound_stop()
{
	SDL_PauseAudio( true );
	
	// be sure audio thread is not active
	SDL_LockAudio();
	SDL_UnlockAudio();
}

static void sound_cleanup()
{
	sound_stop();
	SDL_CloseAudio();
}