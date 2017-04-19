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

#include <string>
#define EXPORT __declspec(dllexport)

const wchar_t *szName          = L"Plugin display name";
const wchar_t *szManufacture   = L"Your name/organization";
const wchar_t *szContact       = L"Your contact information";
const wchar_t *szDescription   = L"Plugin description";
const wchar_t *szCreateDate    = L"Plugin created date, such as 2016-10-11";
const wchar_t *szSuffixes      = L"";// for example "mp3 wma"

// Initialize plugin
bool initialize()
{
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
    return szSuffixes;
}

const wstring suffixDescription(const wstring suffix)
{
    return L"Unknow suffix";
}

// Parse file to get details information
bool parse(wstring file, TMusicInfo* musicInfo)
{
    // Parse file and fill result into musicInfo

    return false;
}

// Load track to prepare for playing
bool loadTrack(TTrackInfo* trackInfo)
{
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

}

// Request next samples
void nextSamples(byte* buffer, int bufSize)
{

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
        *(wstring*)param1 = getLastError();
        break;
    case BC_DESTRORY:
        destroy();
        break;
    default:
        break;
    }
}
