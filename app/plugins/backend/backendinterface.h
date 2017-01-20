/*
 * Copyright (C) Kreogist Dev Team
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

#include <vector>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT __declspec(dllexport)

typedef unsigned int uint;
typedef long int64 ;

#define MAX_PATH 260

struct TPluginInfo
{
    string name;
    string manufacture;
    string contact;
    string description;
    string createDate;
    string fileName;
};

struct TTrackInfo
{
    string trackName; // Track display name
    int index; // Track index id/start Position
    string indexName; // Track index name if index id is invalid
    string musicFileName; // Pointer to musicFileName of TMusicInfo
    string additionalInfo;
    int64 fileSize;
    uint duration;
    TTrackInfo() {
        index = 0;
        fileSize = 0;
        duration = 0;
    }
};

typedef vector<TTrackInfo*> TTrackInfoList;

struct TMusicInfo
{
    string musicName; // Music diplay name
    uint duration;
    int64 fileSize;
    string additionalInfo;
    string musicFileName;
    TTrackInfoList trackList;

    TMusicInfo(){
        fileSize = 0;
        duration = 0;
    }
};

typedef vector<TMusicInfo*> TMusicInfoList;

typedef void (*TRequestSamples)(int dwSamples, short *samples);

// Initialize plugin
EXPORT bool initialize();

// Verify this plugin can parse specify suffix of file
EXPORT const char *matchSuffixes();

// Return description of this suffix, for example "mp3" should be "Moving Picture Experts Group Audio Layer III"
EXPORT const char *suffixDescription(const char *suffix);

// Parse file to get details information
EXPORT bool parse(const char *file, TMusicInfo* musicInfo);

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* track);

// Close track
EXPORT void closeTrack();

// Request next samples
EXPORT void nextSamples(int size, short* samples);

// Retrieve plugin information
EXPORT void pluginInformation(TPluginInfo *pluginInfo);

// Use to free plugin
EXPORT void destroy();

#ifdef __cplusplus
}
#endif
#endif // BACKENDINTERFACE_H
