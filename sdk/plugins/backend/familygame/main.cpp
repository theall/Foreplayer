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

const wchar_t *szName          = L"family game";
const wchar_t *szManufacture   = L"Your name/organization";
const wchar_t *szContact       = L"Your contact information";
const wchar_t *szDescription   = L"Plugin description";
const wchar_t *szCreateDate    = L"";//Plugin created date, such as 2016-10-11
const wchar_t *g_cTypeDesc      = \
        L"ZIP ;Universe package\n" \
        L"RSN ;Super Nintendo package\n" \
        L"AY  ;ZX Spectrum/Amstrad CPC\n" \
        L"GBS ;Nintendo Game Boy\n" \
        L"GYM ;Sega Genesis/Mega Drive\n" \
        L"HES ;NEC TurboGrafx-16/PC Engine\n" \
        L"KSS ;MSX Home Computer/other Z80 systems (doesn't support FM sound)\n" \
        L"NSF ;Nintendo NES/Famicom (with VRC 6, Namco 106, and FME-7 sound)\n" \
        L"SAP ;Atari systems using POKEY sound chip\n" \
        L"SPC ;Super Nintendo/Super Famicom\n" \
        L"VGM ;Sega Master System/Mark III, Sega Genesis/Mega Drive,BBC Micro\n";

TGmeWrap *g_gmePlay; // Gme for play
map<wstring, wstring> g_typeDesc;
wstring g_suffixes;

// Initialize plugin
EXPORT bool initialize()
{
    wstring desc = g_cTypeDesc;

    vector<wstring> itemList;
    split(desc, L"\n", &itemList);
    for(wstring s : itemList) {
        vector<wstring> item;
        split(s, L";", &item);
        if(item.size() != 2)
            continue;

        wstring suffix = item[0];
        trim(suffix);
        lower(suffix);
        g_typeDesc[suffix] = item[1];
        g_suffixes += L" " + suffix;
    }
    g_suffixes.erase(0);

    g_gmePlay = TGmeWrap::instance();

    return true;
}

EXPORT const wstring getLastError()
{
    return L"";
}

// Verify this plugin can parse specify suffix of file
EXPORT const wstring matchSuffixes()
{
    // Return suffix list this plugin can process, multiple suffixed seperated by space character
    return g_suffixes;
}

// Return description of this suffix, for example "mp3" should be "Moving Picture Experts Group Audio Layer III"
EXPORT const wstring suffixDescription(const wstring suffix)
{
    return g_typeDesc[suffix];
}

// Parse file to get details information
EXPORT bool parse(const wstring fileName, TMusicInfo* musicInfo)
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
        if(suffix==L"rsn")
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
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    if(!g_gmePlay || !trackInfo)
        return false;

    bool error = false;
    wstring fileName = trackInfo->musicFileName;
    wstring suffix = extractSuffix(fileName);
    char *data = NULL;
    int size = 0;
    if(suffix==L"rsn" || suffix==L"rar")
    {
        // File is in package
        TRarParse rarParse(fileName);

        rarParse.trackData(trackInfo, &data, &size);
        error = g_gmePlay->loadData(data, size);
    } else if (suffix==L"zip") {
        TZipParse zipParse(fileName);
        zipParse.trackData(trackInfo, &data, &size);
        error = g_gmePlay->loadData(data, size);
    } else {
        error = g_gmePlay->loadFile(fileName.c_str());
    }
    if(error)
        error = g_gmePlay->startTrack(trackInfo->index);
    return error;
}

// Close track
EXPORT void closeTrack()
{
    g_gmePlay->free();
}

// Request next samples
EXPORT void nextSamples(byte* buffer, int bufSize)
{
    if(g_gmePlay)
        g_gmePlay->fillBuffer((short*)buffer, bufSize/2);
}

// Seek time
EXPORT bool seek(int microSeconds)
{
    if(g_gmePlay)
        return g_gmePlay->seek(microSeconds);

    return false;
}

// Retrieve plugin information
EXPORT void pluginInformation(TPluginInfo *pluginInfo)
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
EXPORT void destroy()
{
    if(g_gmePlay) {
        TGmeWrap::deleteInstance();
        g_gmePlay = NULL;
    }
}
