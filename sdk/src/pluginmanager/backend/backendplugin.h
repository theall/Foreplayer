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

#ifndef BACKENDPLUGIN_H
#define BACKENDPLUGIN_H

#include <string>

#include "../library.h"

#include "../../../plugins/backend/backendinterface.h"

class TBackendPlugin
{
public:
    explicit TBackendPlugin();
    ~TBackendPlugin();

    bool load(wstring pluginName);
    bool openTrack(TTrackInfo* track);
    void closeTrack();

    void getNextSamples(byte *buffer, int size);

    bool matchSuffix(wstring suffix);
    map<wstring, wstring> suffixListDescription();
    wstring suffixDescription(wstring suffix);

    bool parse(wstring file, TMusicInfo* musicInfo);

    int getSampleSize(int sampleRate, int fps);
    bool seek(int microSeconds);

    TPluginInfo *pluginInfo();
    const wstring getLastError();

private:
    SEND_CMD_PROC mSendCmd;

    TPluginInfo mPluginInfo;
    vector<wstring> mMatchSuffixes;

    TLibrary *mLibrary;
    wstring mFileName;
    string mLastError;
};
#endif // BACKENDPLUGIN_H
