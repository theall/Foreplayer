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
EXPORT bool initialize()
{
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
    return szSuffixes;
}

// Parse file to get details information
EXPORT bool parse(wstring file, TMusicInfo* musicInfo)
{
    // Parse file and fill result into musicInfo

    return true;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    return true;
}

// Close track
EXPORT void closeTrack()
{

}

// Request next samples
EXPORT void nextSamples(byte* buffer, int bufSize)
{

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
}
