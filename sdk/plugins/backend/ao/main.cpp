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

#include <aosdk.h>

#include "zipparse.h"
#include "pluginutils.h"

#define EXPORT __declspec(dllexport)

const wchar_t *szName = L"Audio overload";
const wchar_t *szManufacture = L"bilge theall";
const wchar_t *szContact = L"wazcd_1608@qq.com";
const wchar_t *szDescription = L"Audio overload plugin.";
const wchar_t *szCreateDate = L"2017-02-18";

struct SuffixDesc
{
    wchar_t *suffix;
    wchar_t *desc;
};

const SuffixDesc g_typeDesc[] =
{
    {(wchar_t*)L"qsf", (wchar_t*)L"Capcom QSound"},
    {(wchar_t*)L"ssf", (wchar_t*)L"Sega Saturn"},
    {(wchar_t*)L"psf", (wchar_t*)L"Sony PlayStation"},
    {(wchar_t*)L"spu", (wchar_t*)L"Sony PlayStation"},
    {(wchar_t*)L"psf2", (wchar_t*)L"Sony PlayStation 2"},
    {(wchar_t*)L"dsf", (wchar_t*)L"Sega Dreamcast"},
    {(wchar_t*)L"minidsf", (wchar_t*)L"Sega Dreamcast Track"},
    {(wchar_t*)L"minipsf", (wchar_t*)L"Game Boy Advance Track"},
    {(wchar_t*)L"zip", (wchar_t*)L"Track list package"},

    {NULL, NULL}
};

wchar_t g_SuffixList[256];
wstring g_error;
wstring g_fileContext;


void libCallback(const char *fileName, char **buffer, long *length)
{
    *buffer = NULL;
    *length = 0;

    wstring fileNameW = char2wstring(fileName);
    wchar_t lastChar = g_fileContext[g_fileContext.size()-1];
    if(lastChar!=L'\\' && lastChar!=L'/')
    {
        // Lib file is in a zip archive package
        TZipParse zipParser(g_fileContext);
        TTrackInfo trackInfo;
        trackInfo.musicFileName = g_fileContext;
        trackInfo.indexName = fileNameW;
        zipParser.trackData(&trackInfo, buffer, (int*)length);
    } else {
        wstring realFileName = g_fileContext + fileNameW;
        readFile(realFileName, buffer, length);
    }
}

// Initialize plugin
bool initialize()
{
    int i = 0;
    g_SuffixList[0] = '\0';
    while (g_typeDesc[i].suffix) {
        wcscat(g_SuffixList, g_typeDesc[i].suffix);
        wcscat(g_SuffixList, L" ");
        i++;
    }
    return true;
}

// Verify this plugin can parse specify suffix of file
const wstring matchSuffixes()
{
    return wstring(g_SuffixList);
}

// Return description of this suffix, for example "mp3" should be "Moving Picture Experts Group Audio Layer III"
const wstring suffixDescription(const wstring suffix)
{
    int i = 0;
    while (g_typeDesc[i].suffix) {
        if(g_typeDesc[i].suffix==suffix)
            return g_typeDesc[i].desc;
        i++;
    }
    return wstring();
}

// Parse file to get details information
bool parse(const wstring fileName, TMusicInfo* musicInfo)
{
    wstring suffix = extractSuffix(fileName);
    int i = 0;
    while(true)
    {
        if(g_typeDesc[i].suffix==suffix || g_typeDesc[i].suffix==NULL)
            break;
        i++;
    }
    if(g_typeDesc[i].suffix == NULL)
        return false;

    bool result = false;
    if(suffix==L"zip")
    {
        TZipParse zipParser(fileName);
        result = zipParser.parse(musicInfo);
    } else {
        TFileParse fileParser(fileName);
        result = fileParser.parse(musicInfo);
    }
    return result;

    //Mission complete.
    return true;
}

// Load track to prepare for playing
bool loadTrack(TTrackInfo* trackInfo)
{
    wstring suffix = extractSuffix(trackInfo->musicFileName);
    if(suffix == L"zip")
    {
        g_fileContext = trackInfo->musicFileName;
        TZipParse zipParser(g_fileContext);
        int fileSize;
        char *buf;
        zipParser.trackData(trackInfo, &buf, &fileSize);
        return ao_load_data(buf, fileSize, libCallback);
    } else {
        g_fileContext = extractPath(trackInfo->musicFileName);
        return ao_load_file(trackInfo->musicFileName.c_str(), libCallback);
    }
    return false;
}

// Seek time
bool seek(int microSeconds)
{
    (void)microSeconds;

    return false;
}

// Close track
void closeTrack()
{
    ao_exit();
    //m1snd_run(M1_CMD_STOP, 0);
}

// Request next samples
void nextSamples( byte* buffer, int bufSize)
{
    // size is the size of 1 channel samples
    ao_do_frame((unsigned long)bufSize/4, buffer);
}

// Optional, for return customized sample size
int sampleSize(int sampleRate, int fps)
{
    // 1 channel
    (void)fps;
    return sampleRate/60;
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
    ao_exit();
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
        *(int*)param3 = sampleSize(*(int*)param1, *(int*)param2);
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
