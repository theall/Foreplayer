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

#include "zipparse.h"

#include "pluginutils.h"

#include <string.h>

#define ZIP_FILE_NAME_SIZE 256
#define ZIP_FILE_MAX_SIZE 4194304

TZipParse::TZipParse(wstring file) :
    TAbstractParse(file)
{
    mSuffixList.push_back(L"ay");
    mSuffixList.push_back(L"gbs");
    mSuffixList.push_back(L"gym");
    mSuffixList.push_back(L"hes");
    mSuffixList.push_back(L"kss");
    mSuffixList.push_back(L"nsf");
    mSuffixList.push_back(L"nsfe");
    mSuffixList.push_back(L"sap");
    mSuffixList.push_back(L"spc");
    mSuffixList.push_back(L"vgz");
    mSuffixList.push_back(L"vgm");
}

TZipParse::~TZipParse()
{

}

bool TZipParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    unzFile zipFile = unzOpen(mFile.c_str());
    if(!zipFile)
        return false;

    int result = unzGoToFirstFile(zipFile);
    if(result != UNZ_OK)
    {
        unzClose(zipFile);
        return false;
    }
    while (true)
    {
        unz_file_info fileInfo;
        memset(&fileInfo, 0, sizeof(unz_file_info));

        char szFileName[ZIP_FILE_NAME_SIZE];
        unzGetCurrentFileInfo(zipFile, &fileInfo, szFileName, ZIP_FILE_NAME_SIZE, NULL, 0, NULL, 0);

        wstring fileNameW = char2wstring(szFileName);
        wstring suffix = extractSuffix(fileNameW);

        // Only process file size < ZIP_FILE_MAX_SIZE
        if(contains(mSuffixList, suffix) && fileInfo.uncompressed_size < ZIP_FILE_MAX_SIZE)
        {
            result = unzOpenCurrentFile(zipFile);
            if(result == UNZ_OK)
            {
                int sizeNeed = fileInfo.uncompressed_size;
                byte *dataBuf = (byte*)malloc(sizeNeed);
                if(unzReadCurrentFile(zipFile, dataBuf, sizeNeed) == sizeNeed)
                {
                    TFileParse fileParse;
                    TTrackInfo *track = fileParse.parse(dataBuf, sizeNeed);
                    if(track)
                    {
                        //__int64 UnpSize = HeaderData.UnpSize+(((__int64)HeaderData.UnpSizeHigh)<<32);
                        //__int64 PackSize = HeaderData.PackSize+(((__int64)HeaderData.PackSizeHigh)<<32);
                        track->fileSize = sizeNeed;
                        track->indexName = fileNameW;
                        musicInfo->duration += track->duration;
                        musicInfo->trackList.push_back(track);
                    }
                }
                delete dataBuf;
                unzCloseCurrentFile(zipFile);
            }
        }
        result = unzGoToNextFile(zipFile);
        if(result != UNZ_OK)
            break;
    }

    unzClose(zipFile);

    if(musicInfo->trackList.size() > 0)
    {
        TTrackInfo* firstTrack = musicInfo->trackList[0];
        musicInfo->game = firstTrack->game;
        musicInfo->artist = firstTrack->artist;
        musicInfo->system = firstTrack->system;
        musicInfo->year = firstTrack->year;
        if(musicInfo->additionalInfo.empty())
            musicInfo->additionalInfo = firstTrack->additionalInfo;
        return true;
    }

    return false;
}

void TZipParse::trackData(TTrackInfo *trackInfo, char **buffer, int *size)
{
    *buffer = NULL;
    *size = 0;
    if(!trackInfo)
        return;

    wstring suffix = extractSuffix(trackInfo->indexName);
    if(!contains(mSuffixList, suffix))
        return;

    unzFile zipFile = unzOpen(trackInfo->musicFileName.c_str());
    if(!zipFile)
        return;

    string indexName = wstring2string(trackInfo->indexName);
    int result = unzLocateFile(zipFile, indexName.c_str(), false);
    if(result != UNZ_OK)
    {
        unzClose(zipFile);
        return;
    }

    result = unzOpenCurrentFile(zipFile);
    if(result == UNZ_OK)
    {
        unz_file_info fileInfo;
        result = unzGetCurrentFileInfo(zipFile, &fileInfo, NULL, 0, NULL, 0, NULL, 0);
        if(result != UNZ_OK)
            return;

        char *buf = (char*)malloc(fileInfo.uncompressed_size);
        int readLength = unzReadCurrentFile(zipFile, buf, fileInfo.uncompressed_size);
        if(readLength==UNZ_ERRNO || readLength!=(int)fileInfo.uncompressed_size)
            return;

        *size = readLength;
        *buffer = buf;
    }
    unzCloseCurrentFile(zipFile);
    unzClose(zipFile);
}
