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

#include "fileparse.h"

#include <string>

#include "pluginutils.h"

TFileParse::TFileParse(wstring file) :
    TAbstractParse(file)
{

}

bool TFileParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    TGmeWrap gme;
    gme.loadFile(mFile.c_str());
    int trackCount = gme.trackCount();
    int trackIndex = 0;
    for(int i=0;i<trackCount;i++) {
        TTrackInfo *trackInfo = new TTrackInfo;
        const gme_info_t *_trackInfo = gme.trackInfo(i);
        trackInfo->trackName = char2wstring(_trackInfo->song);
        if(trackInfo->trackName==L"")
            trackInfo->trackName = L"unknown";
        trackInfo->artist = char2wstring(_trackInfo->author);
        trackInfo->game = char2wstring(_trackInfo->game);
        trackInfo->system = char2wstring(_trackInfo->system);
        trackInfo->additionalInfo = getAdditionalInfo(_trackInfo);
        trackInfo->duration  = _trackInfo->length;
        trackInfo->index     = trackIndex;
        musicInfo->trackList.push_back(trackInfo);
        musicInfo->duration += trackInfo->duration;
        trackIndex++;
    }
    if(trackCount > 0)
    {
        TTrackInfo *trackInfo = musicInfo->trackList.at(0);
        musicInfo->additionalInfo = trackInfo->additionalInfo;
        if(trackCount==1)
            musicInfo->musicName = trackInfo->trackName;
    }

    return true;
}

TTrackInfo *TFileParse::parse(byte *data, int size)
{
    TGmeWrap gme;
    gme.loadData((void*)data, size);
    int trackCount = gme.trackCount();
    if(trackCount != 1)
        return NULL;

    TTrackInfo *trackInfo = new TTrackInfo;
    const gme_info_t *_trackInfo = gme.trackInfo(0);
    trackInfo->trackName = char2wstring(_trackInfo->song);
    trackInfo->index = 0;
    trackInfo->duration  = _trackInfo->length;
    if(trackInfo->duration <= 0)
        trackInfo->duration = _trackInfo->play_length;
    trackInfo->fileSize  = size;
    trackInfo->additionalInfo = getAdditionalInfo(_trackInfo);

    return trackInfo;
}

wstring TFileParse::getAdditionalInfo(const gme_info_t *trackInfo)
{
    char buf[1024];
    buf[0] = 0;
    sprintf(buf, "dumper: %s\n"
            "copyright: %s\n"
            "comment: %s", trackInfo->dumper, trackInfo->copyright, trackInfo->comment);

    return char2wstring(buf);
}
