// Sound output driver using SDL

#include "SDL.h"

static sound_callback_t sound_callback;
static void* sound_callback_data;

static void sdl_callback( void* data, Uint8* out, int count );

static const char* sound_init( long sample_rate, int buf_size,
		sound_callback_t cb, void* data );

static void sound_start();

static void sound_stop();

static void sound_cleanup();