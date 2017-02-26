#include "aosdk.h"

/*
    Audio Overload SDK - main driver.  for demonstration only, not user friendly!

    Copyright (c) 2007-2009 R. Belmont and Richard Bannister.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the names of R. Belmont and Richard Bannister nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ao.h"
#include "eng_protos.h"
#include "m1sdr.h"
#include "aosdk.h"
#include "corlett.h"

/* file types */
volatile ao_bool ao_song_done;
static uint8 *g_buf=NULL;
static TLibCallback g_libCallback=NULL;

typedef struct
{
    uint32 sig;
    char *name;
    int32 (*start)(uint8 *, uint32);
    int32 (*sample)(stereo_sample_t *);
    int32 (*frame)(void);
    int32 (*stop)(void);
    int32 (*command)(int32, int32);
    uint32 rate;
    int32 (*fillinfo)(ao_display_info *);
}Engine;

static Engine g_engines[] =
{
    { 0x50534641, "Capcom QSound (.qsf)", qsf_start, qsf_sample, qsf_frame, qsf_stop, qsf_command, 60, qsf_fill_info },
    { 0x50534611, "Sega Saturn (.ssf)", ssf_start, ssf_sample, ssf_frame, ssf_stop, ssf_command, 60, ssf_fill_info },
    { 0x50534601, "Sony PlayStation (.psf)", psf_start, psf_sample, psf_frame, psf_stop, psf_command, 60, psf_fill_info },
    { 0x53505500, "Sony PlayStation (.spu)", spu_start, spu_sample, spu_frame, spu_stop, spu_command, 60, spu_fill_info },
    { 0x50534602, "Sony PlayStation 2 (.psf2)", psf2_start, psf2_sample, psf2_frame, psf2_stop, psf2_command, 60, psf2_fill_info },
    { 0x50534612, "Sega Dreamcast (.dsf)", dsf_start, dsf_sample, dsf_frame, dsf_stop, dsf_command, 60, dsf_fill_info },

    { 0xffffffff, "", NULL, NULL, NULL, NULL, NULL, 0, NULL }
};

static Engine *g_currentEngine = NULL;

void ao_do_frame(unsigned long sample_count, unsigned char *buffer)
{
    if(!g_currentEngine)
        return;

    unsigned long i;
    stereo_sample_t *p = (stereo_sample_t *)buffer;
    for (i = 0; i < sample_count; i++)
    {
        g_currentEngine->sample(p);
        p++;
    }
    g_currentEngine->frame();
}

void freeEngine()
{
    if(g_currentEngine)
    {
        g_currentEngine->stop();
        g_currentEngine = NULL;
    }
    if(g_buf)
    {
        free(g_buf);
        g_buf = NULL;
    }
}

//// stub for MAME stuff
int change_pc(int foo)
{
    (void)foo;
    return 0;
}

Engine *findModuleByTag(uint8 buffer[4])
{
    uint32 filesig = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
    int type = 0;
    while (g_engines[type].sig != 0xffffffff)
    {
        if (filesig == g_engines[type].sig)
        {
            break;
        }
        else
        {
            type++;
        }
    }

    // now did we identify it above or just fall through?
    if (g_engines[type].sig == 0xffffffff)
        return NULL;

    return &g_engines[type];
}


/* ao_get_lib: called to load secondary files */
int ao_get_lib(const char *filename, uint8 **buffer, uint64 *length)
{
    if(g_libCallback)
    {
        char *buf;
        long n = 0;
        g_libCallback(filename, &buf, &n);
        if(n <= 0)
            return AO_FAIL;

        *buffer = (uint8 *)buf;
        *length = n;
    } else {
        uint8 *filebuf;
        uint32 size;
        FILE *auxfile;

        auxfile = ao_fopen(filename, "rb");
        if (!auxfile)
        {
            printf("Unable to find auxiliary file %s\n", filename);
            return AO_FAIL;
        }

        fseek(auxfile, 0, SEEK_END);
        size = ftell(auxfile);
        fseek(auxfile, 0, SEEK_SET);

        filebuf = malloc(size);

        if (!filebuf)
        {
            fclose(auxfile);
            printf("ERROR: could not allocate %d bytes of memory\n", size);
            return AO_FAIL;
        }

        fread(filebuf, size, 1, auxfile);
        fclose(auxfile);

        *buffer = filebuf;
        *length = (uint64)size;
    }
    return AO_SUCCESS;
}

Engine *loadFile(const wchar_t *fileName, TLibCallback libCallback)
{
    FILE *fp = _wfopen(fileName, L"rb");
    if(!fp)
        return NULL;

    uint8 tag[4];

    // read the file
    fread(tag, 1, 4, fp);

    // now try to identify the file
    Engine *engine = findModuleByTag(tag);
    if(!engine)
    {
        fclose(fp);
        return NULL;
    }

    // get the length of the file by seeking to the end then reading the current position
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    if(g_buf)
        g_buf = (uint8*)realloc(g_buf, fileSize);
    else
        g_buf = (uint8*)malloc(fileSize);

    // reset the pointer
    fseek(fp, 0, SEEK_SET);

    fread(g_buf, 1, fileSize, fp);
    fclose(fp);

    g_libCallback = libCallback;
    if (engine->start(g_buf, fileSize) != AO_SUCCESS)
        engine = NULL;
    g_libCallback = NULL;

    return engine;
}

EXPORT int ao_music_info(const wchar_t *fileName, AO_MUSIC_INFO *music_info)
{
    if(!music_info)
        return 0;

    Engine *engine = loadFile(fileName, NULL);
    if(!engine)
        return 0;

    ao_display_info info;
    if(engine->fillinfo(&info) != AO_SUCCESS)
    {
        return 0;
    }

    strcpy(music_info->name, info.title);
    strcpy(music_info->game, info.game);
    strcpy(music_info->artist, info.artist);
    strcpy(music_info->copyright, info.copyright);
    music_info->year      = atoi(info.year);
    music_info->length    = psfTimeToSeconds(info.length)*1000;
    music_info->fade      = atoi(info.fade);

    return 1;
}

int ao_load_file(const wchar_t *fileName, TLibCallback libCallback)
{
    if(g_currentEngine)
        g_currentEngine->stop();

    g_currentEngine = loadFile(fileName, libCallback);
    if(!g_currentEngine)
        return 0;

    return 1;
}

int ao_play_file(const wchar_t *fileName)
{
    (void)fileName;

#ifdef ENABLE_PLAY
    g_currentEngine = loadFile(fileName);
    if(!g_currentEngine)
        return 0;

    m1sdr_Init(44100);
    m1sdr_PlayStart();

    while (!ao_song_done)
    {
        m1sdr_ret_t ret = M1SDR_OK;
        ret = m1sdr_TimeCheck();
        if(ret == M1SDR_WAIT)
        {
            ao_sleep(50);
        }
    }
#endif
    return 1;
}

void ao_exit()
{
    freeEngine();
}

int ao_load_data(const char *buf, int size, TLibCallback libCallback)
{
    g_libCallback = NULL;

    uint8 tag[4];
    memcpy(tag, buf, 4);
    Engine *engine = findModuleByTag(tag);
    if(g_currentEngine)
        g_currentEngine->stop();

    g_libCallback = libCallback;
    int result = engine->start((uint8*)buf, (uint32)size);
    g_libCallback = NULL;
    if(!engine || result != AO_SUCCESS)
        return 0;

    g_currentEngine = engine;
    return 1;
}
