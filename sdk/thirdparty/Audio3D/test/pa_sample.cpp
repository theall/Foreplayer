
#include <assert.h>
#include <iostream>
#include <vector>

//#include <termios.h>
#include <unistd.h>

#include "audio_3d.h"

#include <portaudio/portaudio.h>
#include <SDL2/SDL.h>
#undef main

const int kSampleRate = 44100;
const int kFramesPerBuffer = 512;

static float elevation_deg = 0;
static float azimuth_deg = 0;
static float distance = 1;
bool distanceDir = true;
bool azimuthDir = true;
bool elevationDir = true;
char *g_SourceBuf;
char *g_pbuf;
int g_count;

static void sdl_callback( void* userData, Uint8* out, int count )
{
    if(g_pbuf-g_SourceBuf>g_count)
        g_pbuf = g_SourceBuf;

    short *in = (short*)g_pbuf;
//    memcpy(out, g_pbuf, count);
//    g_pbuf += count;
//    return;

    unsigned int i;

    Audio3DSource* audio_3d = reinterpret_cast<Audio3DSource*>(userData);
    assert(audio_3d!=0);

//    if(elevationDir)
//        elevation_deg+=1;
//    else
//        elevation_deg-=1;

//    if(elevation_deg >= 360)
//        elevationDir = false;
//    else if(elevation_deg <= -360)
//        elevationDir = true;

//    if(distanceDir)
//        distance += .01;
//    else
//        distance -= .01;
//    if(distance > 2)
//        distanceDir = false;
//    else if(distance < -1)
//        distanceDir = true;

    azimuth_deg += .1;
    if(azimuth_deg >= 10)
    {
        elevation_deg+=1;
        azimuth_deg = -10;
    }


    audio_3d->SetDirection(elevation_deg, azimuth_deg, distance);

    std::vector<float> input(kFramesPerBuffer, 0.0f);
    for( i=0; i<kFramesPerBuffer; i++ )
    {
        float left_sample = *in++;
        float right_sample = *in++;
        input[i] = (left_sample + right_sample) / 2.0;  // Mono downmix
    }

    std::vector<float> output_left;
    std::vector<float> output_right;
    audio_3d->ProcessBlock(input, &output_left, &output_right);

    assert(output_left.size()==kFramesPerBuffer);
    assert(output_right.size()==kFramesPerBuffer);
    short *out16 = (short*)out;
    for (int i=0; i<kFramesPerBuffer; ++i) {
        float outL = output_left[i]*0.75;
        float outR = output_right[i]*0.75;
        float is = input[i] * 0.25;
        float l = is + outL - is * outL/0x10000 + 0.5;
        float r = is + outR - is * outR/0x10000 + 0.5;
        //float l = (output_left[i]+input[i] - (output_left[i]*input[i]/0x10000))/2 + 0.5;
        //float r = (output_right[i]+input[i] - (output_right[i]*input[i]/0x10000))/2 + 0.5;
        if (l > 32767) l = 32767;
        else if (l < -32768) l = -32768;
        *out16++ = (short)l;
        *out16++ = (short)r;
    }
    g_pbuf += count;
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

    Audio3DSource audio_3d(kSampleRate, kFramesPerBuffer);
    static SDL_AudioSpec as; // making static clears all fields to 0
    as.freq     = 44100;
    as.format   = AUDIO_S16SYS;
    as.channels = 2;
    as.callback = sdl_callback;
    as.samples  = kFramesPerBuffer;
    as.userdata = &audio_3d;
    SDL_OpenAudio(&as, 0);

    FILE *fp = fopen("z:/mp3test/test.pcm", "rb");
    fseek (fp, 0, SEEK_END);   ///将文件指针移动文件结尾
    g_count=ftell(fp); ///求出当前文件指针距离文件开始的字节数
    g_SourceBuf = (char*)malloc(g_count);
    g_pbuf = g_SourceBuf;
    fseek (fp, 0, SEEK_SET);   ///将文件指针移动文件结尾
    fread(g_SourceBuf, 1, g_count, fp);
    fclose(fp);

    SDL_PauseAudio(false);
    bool keep_running = true;
    while (keep_running) {
        int c = getchar();
        switch (c) {
        case 27:
            keep_running = false;
            break;
        case 'u':
            elevation_deg += 5;
            break;
        case 'd':
            elevation_deg -= 5;
            break;
        case 'l':
            azimuth_deg -= 5;
            break;
        case 'r':
            azimuth_deg += 5;
            break;
        case '+':
            distance += 1;
            break;
        case '-':
            distance -= 1;
            break;
        default:
            break;
        }
        std::cout<<"Elevation: "<<elevation_deg<<" Azimuth: "<<azimuth_deg<<" Distance: "<<distance <<std::endl;
    }

    return 0;
}
