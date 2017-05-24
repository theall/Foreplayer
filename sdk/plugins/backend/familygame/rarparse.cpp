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

#include "rarparse.h"

#include <string.h>
#include <unrar.h>

#include "pluginutils.h"

#define RAR_FILE_MAX_SIZE 4194304

enum ERR_TYPE {
    ERR_OPEN,
    ERR_READ,
    ERR_PROCESS
};
enum USER_DATA {
    EXTRACT,
    TEST,
    PRINT,
    LIST
};

struct TUserData
{
    char *buffer = NULL;
    int bufSize;
    int sizeNeed;
};

#define BUF_SIZE 16384

void OutError(int Error,char *ArcName,int ErrType)
{
    switch(Error)
    {
        case ERAR_NO_MEMORY:
            printf("\nNot enough memory");
            break;
        case ERAR_BAD_DATA:
            printf("\n%s: archive header or data are damaged", ArcName);
            break;
        case ERAR_BAD_ARCHIVE:
            printf("\n%s is not RAR archive", ArcName);
            break;
        case ERAR_UNKNOWN_FORMAT:
            printf("Unknown archive format");
            break;
        case ERAR_EOPEN:
            if (ErrType==ERR_PROCESS) // Returned by RARProcessFile.
                printf("Volume open error");
            else
                printf("\nCannot open %s",ArcName);
            break;
        case ERAR_ECREATE:
            printf("File create error");
            break;
        case ERAR_ECLOSE:
            printf("File close error");
            break;
        case ERAR_EREAD:
            printf("Read error");
            break;
        case ERAR_EWRITE:
            printf("Write error");
            break;
        case ERAR_SMALL_BUF:
            printf("Buffer for archive comment is too small, comment truncated");
            break;
        case ERAR_UNKNOWN:
            printf("Unknown error");
            break;
        case ERAR_MISSING_PASSWORD:
            printf("Password for encrypted file or header is not specified");
            break;
        case ERAR_EREFERENCE:
            printf("Cannot open file source for reference record");
            break;
        case ERAR_BAD_PASSWORD:
            printf("Wrong password is specified");
            break;
    }
}

int CALLBACK CallbackProc(UINT msg, LPARAM userdata, LPARAM p1, LPARAM p2)
{
    if(msg == UCM_PROCESSDATA)
    {
        TUserData *ud = (TUserData*)userdata;
        if(ud)
        {
            if(ud->bufSize < ud->sizeNeed)
            {
                memcpy(ud->buffer+ud->bufSize, (char*)p1, p2);
                ud->bufSize += p2;
            }

            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

TRarParse::TRarParse(wstring file) :
    TAbstractParse(file)
{

}

TRarParse::~TRarParse()
{

}

bool TRarParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    HANDLE hArcData;
    int RHCode;
    char *cmtBuf = new char[BUF_SIZE];
    TUserData userData;

    RAROpenArchiveDataEx openArchiveData;
    wchar_t RedirName[1024];

    RARHeaderDataEx HeaderData;
    memset(&HeaderData, 0, sizeof(HeaderData));
    memset(&openArchiveData, 0, sizeof(openArchiveData));

    openArchiveData.ArcNameW     = (wchar_t*)mFile.c_str();
    openArchiveData.CmtBuf      = cmtBuf;
    openArchiveData.CmtBufSize  = BUF_SIZE;
    openArchiveData.OpenMode    = RAR_OM_EXTRACT;
    openArchiveData.Callback    = CallbackProc;
    openArchiveData.UserData    = (LPARAM)&userData;
    hArcData = RAROpenArchiveEx(&openArchiveData);

    if (openArchiveData.OpenResult != 0)
    {
        printf("Fail to open file, %d\n", openArchiveData.OpenResult);
        return false;
    }
    HeaderData.RedirName = RedirName;
    HeaderData.RedirNameSize = sizeof(RedirName) / sizeof(RedirName[0]);

    while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0)
    {
        userData.sizeNeed = (int)HeaderData.UnpSize;
        userData.buffer = (char*)realloc(userData.buffer, userData.sizeNeed);
        userData.bufSize = 0;

        wstring currentFile = HeaderData.FileNameW;
        wstring suffix = extractSuffix(currentFile);
        if(suffix == L"spc")
        {
            int error = RARProcessFile(hArcData, RAR_EXTRACT, NULL, NULL);
            if(error == ERAR_SUCCESS && userData.sizeNeed == userData.bufSize)
            {
                wstring curentFileL = toLower(currentFile);
                if(curentFileL==L"info.txt")
                    musicInfo->additionalInfo = char2wstring(userData.buffer);
                else {
                    TFileParse fileParse;
                    TTrackInfo *track = fileParse.parse((byte*)userData.buffer, userData.bufSize);
                    if(track)
                    {
                        //__int64 UnpSize = HeaderData.UnpSize+(((__int64)HeaderData.UnpSizeHigh)<<32);
                        //__int64 PackSize = HeaderData.PackSize+(((__int64)HeaderData.PackSizeHigh)<<32);
                        track->fileSize = HeaderData.UnpSize;
                        track->indexName = currentFile;
                        musicInfo->duration += track->duration;
                        musicInfo->trackList.push_back(track);
                    }
                }
            } else {
                printf("Error in processing rar file: %d\n", error);
            }
        } else {
            RARProcessFile(hArcData, RAR_SKIP, NULL, NULL);
        }
    }
    RARCloseArchive(hArcData);

    if(userData.buffer)
    {
        delete userData.buffer;
        userData.buffer = NULL;
    }

    delete cmtBuf;

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

void TRarParse::trackData(TTrackInfo *trackInfo, char **buffer, int *size)
{
    *buffer = NULL;
    *size = 0;
    if(!trackInfo)
        return;

    HANDLE hArcData;
    int RHCode;
    char *cmtBuf = new char[BUF_SIZE];
    TUserData userData;

    RAROpenArchiveDataEx openArchiveData;
    wchar_t RedirName[1024];

    RARHeaderDataEx HeaderData;
    memset(&HeaderData, 0, sizeof(HeaderData));
    memset(&openArchiveData, 0, sizeof(openArchiveData));

    openArchiveData.ArcNameW     = (wchar_t*)mFile.c_str();
    openArchiveData.CmtBuf      = cmtBuf;
    openArchiveData.CmtBufSize  = BUF_SIZE;
    openArchiveData.OpenMode    = RAR_OM_EXTRACT;
    openArchiveData.Callback    = CallbackProc;
    openArchiveData.UserData    = (LPARAM)&userData;
    hArcData = RAROpenArchiveEx(&openArchiveData);

    if (openArchiveData.OpenResult != 0)
    {
        printf("Fail to open file, %d\n", openArchiveData.OpenResult);
        return;
    }
    HeaderData.RedirName = RedirName;
    HeaderData.RedirNameSize = sizeof(RedirName) / sizeof(RedirName[0]);

    while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0)
    {
        if(trackInfo->indexName==HeaderData.FileNameW && HeaderData.UnpSize<RAR_FILE_MAX_SIZE)
        {
            userData.sizeNeed = (int)HeaderData.UnpSize;
            userData.sizeNeed = (int)HeaderData.UnpSize;
            userData.buffer = (char*)realloc(userData.buffer, userData.sizeNeed);
            userData.bufSize = 0;
            int error = RARProcessFile(hArcData, RAR_EXTRACT, NULL, NULL);
            if(error == ERAR_SUCCESS && userData.sizeNeed == userData.bufSize)
            {
                *buffer = userData.buffer;
                *size = userData.bufSize;
            } else {
                printf("%d\n", error);
            }
            break;
        } else {
            RARProcessFile(hArcData, RAR_SKIP, NULL, NULL);
        }
    }
    RARCloseArchive(hArcData);

    delete cmtBuf;
}
