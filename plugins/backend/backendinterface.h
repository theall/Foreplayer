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

#include <list>
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
    string musicFullName;
    string trackName;
    int index;
    int64 size;
    int64 startPosition;       //Music properties.
    uint duration;
    uint bitRate;
    uint samplingRate;
};

struct TMusicInfo
{
    string musicFileName;
    string musicFullName;
    string musicName;
    string album;
    string artist;
    string date;
    string musicType;
    string musicTypeDesc;
    list<TTrackInfo*> trackList;
};

void (*TRequestSamples)(int dwSamples, short *samples);

// Initialize plugin
EXPORT bool initialize();

// Verify this plugin can parse specify suffix of file
EXPORT const string matchSuffixes();

// Parse file to get details information
EXPORT bool parse(string file, TMusicInfo* musicInfo);

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
