#ifndef SDL_SOUND
#define SDL_SOUND

// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
//============================================================
//
//  sound.c - SDL implementation of MAME sound routines
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

#include <stdio.h>

// standard sdl header
#include <SDL2/SDL.h>

//============================================================
//  DEBUGGING
//============================================================

#define LOG_SOUND       0

//============================================================
//  PROTOTYPES
//============================================================

static void sdl_callback(void *userdata, Uint8 *stream, int len);

//============================================================
//  CLASS
//============================================================

class sound_sdl
{
public:

    friend void sdl_callback(void *userdata, Uint8 *stream, int len);

    // number of samples per SDL callback
    static const int SDL_XFER_SAMPLES = 512;

    sound_sdl() :
        stream_in_initialized(0),
        stream_loop(0),
        attenuation(0),
        mSampleRate(44100),
        buf_locked(0),
        stream_buffer(NULL),
        stream_playpos(0),
        stream_buffer_size(0),
        stream_buffer_in(0),
        buffer_underflows(0),
        buffer_overflows(0)
    {
        sdl_xfer_samples = SDL_XFER_SAMPLES;
    }
    virtual ~sound_sdl() { }

    virtual int init(const int sample_rate);
    virtual void exit();

    // sound_module

    virtual void update_audio_stream(bool is_throttled, const int16_t *buffer, int samples_this_frame);
    virtual void set_mastervolume(int attenuation);

private:
    int lock_buffer(bool is_throttled, long offset, long size, void **buffer1, long *length1, void **buffer2, long *length2);
    void unlock_buffer(void);
    void att_memcpy(void *dest, const int16_t *data, int bytes_to_copy);
    void copy_sample_data(bool is_throttled, const int16_t *data, int bytes_to_copy);
    int sdl_create_buffers(void);
    void sdl_destroy_buffers(void);

    int sdl_xfer_samples;
    int stream_in_initialized;
    int stream_loop;
    int attenuation;
    int mSampleRate;
    int              buf_locked;

    int8_t             *stream_buffer;
    volatile int32_t   stream_playpos;

    uint32_t           stream_buffer_size;
    uint32_t           stream_buffer_in;

    // buffer over/underflow counts
    int              buffer_underflows;
    int              buffer_overflows;


};

#endif // SDL_SOUND

