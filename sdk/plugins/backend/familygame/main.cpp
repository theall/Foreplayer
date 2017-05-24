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

#include <string>

#include "fileparse.h"
#include "rarparse.h"
#include "zipparse.h"

#include "pluginutils.h"

#define EXPORT __declspec(dllexport)

const wchar_t *szName          = L"Gme plugin";
const wchar_t *szManufacture   = L"Bilge Theall";
const wchar_t *szContact       = L"wazcd_1608@qq.com";
const wchar_t *szDescription   = L"Plugin port to gme";
const wchar_t *szCreateDate    = L"2016/10/22";//Plugin created date, such as 2016-10-11
const wchar_t *g_cTypeDesc      = \
        L"ZIP ;Gme zip package\n" \
        L"RAR ;Gme rar package\n" \
        L"RSN ;Spc tracks package\n" \
        L"AY  ;ZX Spectrum/Amstrad CPC\n" \
        L"GBS ;Nintendo Game Boy\n" \
        L"GYM ;Sega Genesis/Mega Drive\n" \
        L"HES ;NEC TurboGrafx-16/PC Engine\n" \
        L"KSS ;MSX Home Computer/other Z80 systems (doesn't support FM sound)\n" \
        L"NSF ;Nintendo NES/Famicom (with VRC 6, Namco 106, and FME-7 sound)\n" \
        L"SAP ;Atari systems using POKEY sound chip\n" \
        L"SPC ;Super Nintendo/Super Famicom\n" \
        L"VGM ;Sega Master System/Mark III, Sega Genesis/Mega Drive,BBC Micro";

static TGmeWrap *g_gmePlayer; // Gme for play
static map<wstring, wstring> g_typeDesc;
static wstring g_suffixes;

// Initialize plugin
bool initialize()
{
    wstring desc = g_cTypeDesc;

    vector<wstring> itemList;
    split(desc, L"\n", &itemList);
    for(wstring s : itemList) {
        vector<wstring> item;
        split(s, L";", &item);
        if(item.size() != 2)
            continue;

        wstring suffix = toLower(item[0]);
        trim(suffix);
        g_typeDesc[suffix] = item[1];
        g_suffixes += L" " + suffix;
    }
    g_suffixes.erase(g_suffixes.begin());

    g_gmePlayer = TGmeWrap::instance();

    return true;
}

const wstring getLastError()
{
    return L"";
}

// Verify this plugin can parse specify suffix of file
const wstring matchSuffixes()
{
    // Return suffix list this plugin can process, multiple suffixed seperated by space character
    return g_suffixes;
}

// Return description of this suffix, for example "mp3" should be "Moving Picture Experts Group Audio Layer III"
const wstring suffixDescription(const wstring suffix)
{
    return g_typeDesc[suffix];
}

// Parse file to get details information
bool parse(const wstring fileName, TMusicInfo* musicInfo)
{
    if(!musicInfo)
        return false;

    // Parse file and fill result into musicInfo
    wstring suffix = extractSuffix(fileName);
    bool result = false;
    TFileParse fileParse(fileName);
    TRarParse rarParse(fileName);
    TZipParse zipParse(fileName);
    if(!g_typeDesc[suffix].empty())
    {
        if(suffix==L"rsn" || suffix==L"rar")
            result = rarParse.parse(musicInfo);
        else if(suffix==L"zip")
            result = zipParse.parse(musicInfo);
        else
            result = fileParse.parse(musicInfo);
    } else {
        // For unknown suffix file, firstly try to verify whether it is a rsn file
        result = rarParse.parse(musicInfo);
        if(!result)
        {
            result = zipParse.parse(musicInfo);
            if(!result)
                result = fileParse.parse(musicInfo);
        }
    }
    return result;
}

// Load track to prepare for playing
bool loadTrack(TTrackInfo* trackInfo)
{
    if(!g_gmePlayer || !trackInfo)
        return false;

    bool success = false;
    wstring fileName = trackInfo->musicFileName;
    wstring suffix = extractSuffix(fileName);
    char *data = NULL;
    int size = 0;
    if(suffix==L"rsn" || suffix==L"rar")
    {
        // File is in package
        TRarParse rarParse(fileName);
        rarParse.trackData(trackInfo, &data, &size);
        if(size < 1)
            return false;

        success = g_gmePlayer->loadData(data, size);
    } else if (suffix==L"zip") {
        TZipParse zipParse(fileName);
        zipParse.trackData(trackInfo, &data, &size);
        if(size < 1)
            return false;

        success = g_gmePlayer->loadData(data, size);
    } else {
        success = g_gmePlayer->loadFile(fileName.c_str());
    }
    if(success)
        success = g_gmePlayer->startTrack(trackInfo->index);

    return success;
}

// Close track
void closeTrack()
{
    g_gmePlayer->free();
}

// Request next samples
void nextSamples(byte* buffer, int bufSize)
{
    if(g_gmePlayer)
        g_gmePlayer->fillBuffer((short*)buffer, bufSize/2);
}

// Seek time
bool seek(int microSeconds)
{
    if(g_gmePlayer)
        return g_gmePlayer->seek(microSeconds);

    return false;
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
    if(g_gmePlayer) {
        TGmeWrap::deleteInstance();
        g_gmePlayer = NULL;
    }
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
        *(bool*)param2 = false;//seek(*(int*)param1);
        break;
    case BC_GET_PLUGIN_INFORMATION:
        pluginInformation((TPluginInfo*)param1);
        break;
    case BC_GET_LAST_ERROR:
        *(wstring*)param1 = L"";
        break;
    case BC_DESTRORY:
        destroy();
        break;
    default:
        break;
    }
}
