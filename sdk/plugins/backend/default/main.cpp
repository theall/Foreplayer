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

#include <backendinterface.h>

#include "bass.h"
#include "pluginutils.h"

#include <string>

#define EXPORT __declspec(dllexport)

const wchar_t *szName = L"Bass";
const wchar_t *szManufacture = L"Bilge Theall";
const wchar_t *szContact = L"wazcd_1608@qq.com";
const wchar_t *szDescription = L"Bass plugin.http://www.un4seen.com/";
const wchar_t *szCreateDate = L"2017-02-19";
const int g_sampleRate = 44100;

HSTREAM g_handle;
wstring g_error;

struct SuffixDesc
{
    wchar_t *suffix;
    wchar_t *desc;
};

const SuffixDesc szTypeDesc[] =
{
    {(wchar_t*)L"wav", (wchar_t*)L"Wave"},
    {(wchar_t*)L"mp3", (wchar_t*)L"Mpeg 1 layer 3"},

    {NULL, NULL}
};

wchar_t g_SuffixList[256];
wstring szError;

// Initialize plugin
bool initialize()
{
    int i = 0;
    g_SuffixList[0] = '\0';
    while (szTypeDesc[i].suffix) {
        wcscat(g_SuffixList, szTypeDesc[i].suffix);
        wcscat(g_SuffixList, L" ");
        i++;
    }
    g_SuffixList[wcslen(g_SuffixList)-1] = 0;

    if (!BASS_Init(0, g_sampleRate, 0, 0, NULL))
        return false;

    return true;
}

// Verify this plugin can parse specify suffix of file
const wstring matchSuffixes()
{
    return wstring(g_SuffixList);
}

const wstring suffixDescription(const wstring suffix)
{
    wstring sf = toLower(suffix);
    int i=0;
    while(szTypeDesc[i].suffix)
    {
        if(szTypeDesc[i].suffix==sf)
            return wstring(szTypeDesc[i].desc);
        i++;
    }
    return L"Unknow suffix";
}

// Parse file to get details information
bool parse(const wstring file, TMusicInfo* musicInfo)
{
    HSAMPLE handle = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, BASS_STREAM_DECODE);
    if(!handle)
        return false;

    TAG_ID3 *id3 = (TAG_ID3*)BASS_ChannelGetTags(handle, BASS_TAG_ID3); // get the ID3 tags
    TTrackInfo *trackInfo = new TTrackInfo;

    if(!id3)
    {
        id3 = (TAG_ID3*)BASS_ChannelGetTags(handle, BASS_TAG_ID3);
        if(id3)
        {
            trackInfo->trackName = char2wstring(id3->title);
            trackInfo->artist = char2wstring(id3->artist);
            trackInfo->game = char2wstring(id3->album);
            trackInfo->year = atoi(id3->year);
            trackInfo->artist = char2wstring(id3->artist);
            trackInfo->additionalInfo = char2wstring(id3->comment);
        }
    }

    if(trackInfo->trackName==L"")
        trackInfo->trackName = extractBaseName(file);

    DWORD pos = BASS_ChannelGetLength(handle, BASS_POS_BYTE);
    if (pos && pos!=(DWORD)-1) {
        musicInfo->fileSize = pos;
        DWORD seconds = (DWORD)BASS_ChannelBytes2Seconds(handle, pos);
        trackInfo->duration = seconds * 1000;
    }

    musicInfo->musicName = trackInfo->trackName;
    musicInfo->duration = trackInfo->duration;
    musicInfo->additionalInfo = trackInfo->additionalInfo;

    musicInfo->trackList.push_back(trackInfo);

    BASS_StreamFree(handle);

    //Mission complete.
    return true;
}

// Load track to prepare for playing
bool loadTrack(TTrackInfo* trackInfo)
{
    if(g_handle)
        BASS_StreamFree(g_handle);

    g_handle = BASS_StreamCreateFile(FALSE, trackInfo->musicFileName.c_str(), 0, 0, BASS_STREAM_DECODE);
    return g_handle != 0;
}

// Seek time
bool seek(int microSeconds)
{
    if(g_handle)
    {
        int bytes = BASS_ChannelSeconds2Bytes(g_handle, (double)microSeconds/1000);
        return BASS_ChannelSetPosition(g_handle, bytes, BASS_POS_BYTE);
    }
    return false;
}

// Close track
void closeTrack()
{
    if(g_handle)
    {
        BASS_StreamFree(g_handle);
        g_handle = 0;
    }
}

// Request next samples
void nextSamples(byte* buffer, int bufSize)
{
    if(g_handle && BASS_ChannelIsActive(g_handle))
        BASS_ChannelGetData(g_handle, buffer, (DWORD)bufSize);
}

// Retrieve plugin information
void pluginInformation(TPluginInfo *pluginInfo)
{
    if(!pluginInfo)
        return;

    pluginInfo->name = szName;
    pluginInfo->manufacture = szManufacture;
    pluginInfo->contact = szContact;
    pluginInfo->description = szDescription;
    pluginInfo->createDate = szCreateDate;
}

// Use to free plugin
void destroy()
{
    closeTrack();
}

EXPORT void send_cmd(
    BackendCmd cmd,
    void *param1,
    void *param2,
    void *param3,
    void *param4)
{
    (void)param4;
    switch (cmd) {
    case BC_INITIALIZE:
        *(bool*)param1 = initialize();
        break;
    case BC_GET_MATCH_SUFFIXES:
        *(wstring*)param1 = matchSuffixes();
        break;
    case BC_GET_SUFFIX_DESCRIPTION:
        *(wstring*)param2 = suffixDescription(*(wstring*)param1);
        break;
    case BC_PARSE:
        *(bool*)param3 = parse(*(wstring*)param1, (TMusicInfo*)param2);
        break;
    case BC_LOAD_TRACK:
        *(bool*)param2 = loadTrack((TTrackInfo*)param1);
        break;
    case BC_CLOSE_TRACK:
        closeTrack();
        break;
    case BC_GET_NEXT_SAMPLES:
        nextSamples((byte*)param1, *(int*)param2);
        break;
    case BC_GET_SAMPLE_SIZE:
        break;
    case BC_SEEK:
        *(bool*)param2 = seek(*(int*)param1);
        break;
    case BC_GET_PLUGIN_INFORMATION:
        pluginInformation((TPluginInfo*)param1);
        break;
    case BC_GET_LAST_ERROR:
        *(wstring*)param1 = g_error;
        break;
    case BC_DESTRORY:
        destroy();
        break;
    default:
        break;
    }
}
