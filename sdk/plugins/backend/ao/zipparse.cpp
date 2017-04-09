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

#include <algorithm>

#define ZIP_FILE_NAME_SIZE 256
#define ZIP_FILE_BUF_SIZE 16384
#define ZIP_FILE_MAX_SIZE 4194304

TZipParse::TZipParse(wstring file) :
    TAbstractParse(file)
  , mLibBuf(NULL)
{
    mSuffixList.push_back("minidsf");
    mSuffixList.push_back("minipsf");
}

TZipParse::~TZipParse()
{
    if(mLibBuf)
    {
        delete mLibBuf;
        mLibBuf = NULL;
    }
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

        // Only process file size < ZIP_FILE_MAX_SIZE
        if(contains(mSuffixList, extractSuffix(szFileName)) && fileInfo.uncompressed_size < ZIP_FILE_MAX_SIZE)
        {
            result = unzOpenCurrentFile(zipFile);
            if(result == UNZ_OK)
            {
                char *dataBuf;
                int sizeNeed = fileInfo.uncompressed_size;
                dataBuf = (char*)malloc(sizeNeed);
                if(unzReadCurrentFile(zipFile, dataBuf, sizeNeed) == sizeNeed)
                {
                    TFileParse fileParse;
                    TTrackInfo *track = fileParse.parse(dataBuf, sizeNeed);
                    if(track)
                    {
                        //__int64 UnpSize = HeaderData.UnpSize+(((__int64)HeaderData.UnpSizeHigh)<<32);
                        //__int64 PackSize = HeaderData.PackSize+(((__int64)HeaderData.PackSizeHigh)<<32);
                        track->fileSize = sizeNeed;
                        track->indexName = char2wstring(szFileName);
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
        musicInfo->additionalInfo = musicInfo->trackList[0]->additionalInfo;
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

    unzFile zipFile = unzOpen(trackInfo->musicFileName.c_str());
    if(!zipFile)
        return;

    int result = unzLocateFile(zipFile, wstring2string(trackInfo->indexName).c_str(), false);
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
        int readLength = unzReadCurrentFile(zipFile, buf, ZIP_FILE_BUF_SIZE);
        if(readLength==UNZ_ERRNO || readLength!=fileInfo.uncompressed_size)
            return;

        *size = readLength;
        *buffer = buf;
    }
    unzCloseCurrentFile(zipFile);
    unzClose(zipFile);
}
