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

#ifndef BACKENDINTERFACE_H
#define BACKENDINTERFACE_H

#include <map>
#include <vector>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

typedef long long int64;

struct TPluginInfo
{
    wstring name;
    wstring manufacture;
    wstring contact;
    wstring description;
    wstring createDate;
    wstring fileName;
};

struct TTrackInfo
{
    wstring trackName; // Track display name
    int index; // Track index id/start Position
    wstring indexName; // Track index name if index id is invalid
    wstring musicFileName; // Pointer to musicFileName of TMusicInfo
    wstring additionalInfo;
    int64 fileSize;
    wstring game;
    wstring artist;
    wstring system;
    int year;
    int64 duration;
    int sampleRate;
    int channels;
    TTrackInfo() {
        index = -1;
        fileSize = 0;
        duration = 0;
        channels = 2;
        sampleRate = 44100;
        trackName = L"unknown";
        year = 0;
    }
};

typedef vector<TTrackInfo*> TTrackInfoList;

struct TMusicInfo
{
    wstring musicName; // Music diplay name
    wstring musicFileName;
    wstring game;
    wstring artist;
    wstring system;
    int year;
    int64 duration;
    int64 fileSize;
    int sampleRate;
    int channels;
    wstring additionalInfo;
    TTrackInfoList trackList;
    TMusicInfo(){
        year = 0;
        fileSize = 0;
        duration = 0;
        channels = 2;
        sampleRate = 44100;
        musicName = L"unknown";
    }
};

typedef vector<TMusicInfo*> TMusicInfoList;

typedef unsigned char byte;
//typedef void (*TRequestSamples)(int bufSize, byte *buffer);

//// Initialize plugin
//bool initialize();

//// Verify this plugin can parse specify suffix of file
//const wstring matchSuffixes();

//// Return description of this suffix, for example "mp3" should be "Moving Picture Experts Group Audio Layer III"
//const wstring suffixDescription(const wstring suffix);

//// Parse file to get details information
//bool parse(const wstring fileName, TMusicInfo* musicInfo);

//// Load track to prepare for playing
//bool loadTrack(TTrackInfo* track);

//// Close track
//void closeTrack();

//// Request next samples
//void nextSamples(byte* buffer, int bufSize);

//// Optional, for return customized sample size
//int sampleSize(int sampleRate, int fps);

//// Seek time
//bool seek(int microSeconds);

//// Retrieve plugin information
//void pluginInformation(TPluginInfo *pluginInfo);

//// Return last error wstring if failed
//const wstring getLastError();

//// Use to free plugin
//void destroy();

enum BackendCmd
{
    /**
     * Initialize plugin
     * param1: return bool
     */
    BC_INITIALIZE,

    /**
     * Retreive suffix list that this plugin can match
     * param1: return wstring
     */
    BC_GET_MATCH_SUFFIXES,

    /**
     * Get description of suffix
     * param1: wstring of suffix
     * param2: return wstring
     */
    BC_GET_SUFFIX_DESCRIPTION,

    /**
     * Parse a music file
     * param1: music file full path
     * param2: pointer to music info struct
     * param3: return bool
     */
    BC_PARSE,

    /**
     * Load track
     * param1: pointer to TTrackInfo
     * param2: return bool
     */
    BC_LOAD_TRACK,

    /**
     * Close current track
     * param1: return bool
     */
    BC_CLOSE_TRACK,

    /**
     * Request next samples
     * param1: pointer to buffer
     * param2: samples size in byte
     */
    BC_GET_NEXT_SAMPLES,

    /**
     * Query sample size per frame
     * param1: sample rate
     * param2: frame frequence
     * param3: return int
     */
    BC_GET_SAMPLE_SIZE,

    /**
     * Seek in current track
     * param1: length in micro seconds to seek
     * param2: return bool
     */
    BC_SEEK,

    /**
     * Retreive plugin information
     * param1: pointer to plugin info struct
     */
    BC_GET_PLUGIN_INFORMATION,

    /**
     * Get last error
     * param1: return wstring
     */
    BC_GET_LAST_ERROR,

    /**
     * Destroy plugin
     */
    BC_DESTRORY
};

#define SEND_CMD_PROC_NAME      "send_cmd"

void send_cmd(BackendCmd cmd,
              void *param1,
              void *param2,
              void *param3,
              void *param4);

typedef void (*SEND_CMD_PROC)(BackendCmd cmd,
                              void *param1,
                              void *param2,
                              void *param3,
                              void *param4);

#ifdef __cplusplus
}
#endif
#endif // BACKENDINTERFACE_H
