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
#include "backendplugin.h"

#include "../../cplusutils.h"

TBackendPlugin::TBackendPlugin() :
    mSendCmd(NULL),
    mLibrary(NULL)
{

}

TBackendPlugin::~TBackendPlugin()
{
    if(mSendCmd)
    {
        mSendCmd(BC_DESTRORY, 0, 0, 0, 0);
        mSendCmd = NULL;
    }
    if(mLibrary)
    {
        delete mLibrary;
        mLibrary = NULL;
    }
}

bool TBackendPlugin::load(wstring pluginName)
{
    mLibrary = new TLibrary(pluginName);
    mSendCmd = (SEND_CMD_PROC)mLibrary->resolve(SEND_CMD_PROC_NAME);
    if(!mSendCmd)
    {
        char buf[1024];
        sprintf(buf, "Can't resolve function %s", SEND_CMD_PROC_NAME);
        mLastError = buf;
        return false;
    }

    // Initialize plugin
    bool bInitialized = false;
    mSendCmd(BC_INITIALIZE, &bInitialized, 0, 0, 0);
    if(!bInitialized)
        return false;

    // Get match suffixes
    wstring matchSuffixes;
    mSendCmd(BC_GET_MATCH_SUFFIXES, &matchSuffixes, 0, 0, 0);
    split(matchSuffixes, L" ", &mMatchSuffixes);
    if(mMatchSuffixes.empty())
        return false;

    // Plugin information
    mSendCmd(BC_GET_PLUGIN_INFORMATION, &mPluginInfo, 0, 0, 0);

    mFileName = pluginName;

    return true;
}

bool TBackendPlugin::openTrack(TTrackInfo *track)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(BC_LOAD_TRACK, track, &ret, 0, 0);

    return ret;
}

void TBackendPlugin::closeTrack()
{
    if(mSendCmd)
        mSendCmd(BC_CLOSE_TRACK, 0, 0, 0, 0);
}

void TBackendPlugin::getNextSamples(byte *buffer, int size)
{
    if(mSendCmd)
        mSendCmd(BC_GET_NEXT_SAMPLES, buffer, &size, 0, 0);
}

bool TBackendPlugin::matchSuffix(wstring suffix)
{
    return contains(mMatchSuffixes, suffix);
}

map<wstring, wstring> TBackendPlugin::suffixListDescription()
{
    map<wstring, wstring> sd;
    for(wstring s : mMatchSuffixes)
        sd[s] = suffixDescription(s);

    return sd;
}

wstring TBackendPlugin::suffixDescription(wstring suffix)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(BC_GET_SUFFIX_DESCRIPTION, &suffix, &ret, 0, 0);

    return ret;
}

bool TBackendPlugin::parse(wstring file, TMusicInfo *musicInfo)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(BC_PARSE, &file, musicInfo, &ret, 0);

    return ret;
}

int TBackendPlugin::getSampleSize(int sampleRate, int fps)
{
    int size = 4096;
    if(mSendCmd)
        mSendCmd(BC_GET_SAMPLE_SIZE, &sampleRate, &fps, &size, 0);

    return size;
}

bool TBackendPlugin::seek(int microSeconds)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(BC_SEEK, &microSeconds, &ret, 0, 0);

    return ret;
}

TPluginInfo *TBackendPlugin::pluginInfo()
{
    return &mPluginInfo;
}

const wstring TBackendPlugin::getLastError()
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(BC_GET_LAST_ERROR, &ret, 0, 0, 0);

    return ret;
}

