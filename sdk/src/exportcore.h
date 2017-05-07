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
#ifndef TEXPORTCORE_H
#define TEXPORTCORE_H

#include "pluginmanager/backendpluginmanager.h"
#include "playlist/trackitem.h"

class TExportCore
{
public:
    TExportCore();
    ~TExportCore();

    // For retrieve samples
    bool loadTrack(TTrackItem *trackItem);
    int samplesPerFrame(int sampleRate, int fps);
    void nextSamples(byte *buffer, int bufSize);

private:
    TBackendPlugin *mCurrentPlugin;
    TBackendPluginManager *mPluginManager;
};

#endif // TEXPORTCORE_H
