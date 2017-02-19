#ifndef AOSDK_H
#define AOSDK_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT __declspec(dllexport)

typedef struct
{
    char *name;
    char *game;
    char *artist;
    char *copyright;
    int year;
    int length;
    int fade;
} AO_MUSIC_INFO;

EXPORT int ao_music_info(const wchar_t *fileName, AO_MUSIC_INFO *music_info);
EXPORT int ao_load_file(const wchar_t *fileName);
EXPORT int ao_load_data(const char *buf, int size);
EXPORT void ao_do_frame(unsigned long sample_count, unsigned char *buffer);
EXPORT int ao_play_file(const wchar_t *fileName);
EXPORT void ao_exit();

#ifdef __cplusplus
}
#endif
#endif // AOSDK_H
