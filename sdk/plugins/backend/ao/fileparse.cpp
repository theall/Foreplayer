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
#include "pluginutils.h"

#include <string>
#include <vector>
#include <algorithm>

#include <string.h>
#include <memory.h>
#include <math.h>

using namespace std;

typedef uint32_t uint;

struct TAG
{
    uint id;
    char *desc;
};

static TAG g_ValidFid[] =
{
    { 0x50534641, (char*)"Capcom QSound" },
    { 0x50534611, (char*)"Sega Saturn" },
    { 0x50534601, (char*)"Sony PlayStation" },
    { 0x53505500, (char*)"Sony PlayStation" },
    { 0x50534602, (char*)"Sony PlayStation 2" },
    { 0x50534612, (char*)"Sega Dreamcast" },

    { 0xffffffff, NULL }
};

bool isValidFile(char *tag)
{
    uint id = (uint)tag[0]<<24|(uint)tag[1]<<16|(uint)tag[2]<<8|(uint)tag[3];
    int i = 0;
    while(true)
    {
        if(g_ValidFid[i].id==id || g_ValidFid[i].id==0xffffffff)
            break;
        i++;
    }
    return g_ValidFid[i].id!=0xffffffff;
}

unsigned int toMicroSeconds(const char *timeStr)
{
    if(!timeStr)
        return 0;

    char tm[3][3];
    memset(tm, 0, 9);

    char *pt = (char*)timeStr;
    int len = strlen(timeStr);
    int iArray = 0;
    int aIndex = 0;
    int i;
    if(len>11)
        return 0;

    int msecs = 0;
    for(i=0;i<len;i++)
    {
        char c = *pt;
        if(c==':')
        {
            tm[iArray][aIndex] = 0;
            aIndex = 0;
            iArray++;
            pt++;
            continue;
        } else if(c=='.') {
            tm[iArray][aIndex] = 0;
            msecs = atoi(pt+1);
            break;
        }
        tm[iArray][aIndex] = c;
        aIndex++;
        pt++;
    }
    iArray++;

    int r = 3 - iArray;
    int microSecs = 0;
    i = 0;
    while(i<3)
    {
        microSecs += atoi(tm[i])*pow(60, 2-i-r);
        i++;
    }
    return microSecs*1000+msecs;
}

void split(string& s, char delim, vector<string> *ret)
{
    size_t last = 0;
    size_t index = s.find_first_of(delim,last);
    while (index!=wstring::npos)
    {
        ret->push_back(s.substr(last,index-last));
        last = index+1;
        index = s.find_first_of(delim,last);
    }
    if (index-last>0)
    {
        ret->push_back(s.substr(last,index-last));
    }
}

void replace(string &s, string src, string rep)
{
    string::size_type startpos = 0;
    int srcLen = src.size();
    while(startpos!= wstring::npos)
    {
        startpos = s.find(src);
        if( startpos != string::npos )
        {
            s.replace(startpos, srcLen, rep);
        }
    }
}

string join(vector<string> sl, string delim)
{
    string ret;

    if(sl.size() <= 0)
        return ret;

    string last = sl[sl.size()-1];
    sl.pop_back();
    for(string s : sl)
    {
        ret += s + delim;
    }
    return ret + last;
}

void fillTrackInfo(string s, TTrackInfo *info)
{
    vector<string> sl;
    split(s, '\n', &sl);
    if(!sl.empty())
    {
        vector<string> otherItems;
        for(string item : sl)
        {
            vector<string> keyValue;
            split(item, '=', &keyValue);
            if(keyValue.size() != 2)
                continue;

            string key = keyValue[0];
            transform(key.begin(), key.end(), key.begin(), ::tolower);

            wstring value = wstring(keyValue[1].begin(), keyValue[1].end());
            if(key=="title")
            {
                info->trackName = value;
            } else if (key=="length") {
                info->duration = toMicroSeconds(keyValue[1].c_str());
            }  else if (key=="game") {
                info->game = value;
            } else if (key=="year") {
                info->year = atoi(keyValue[1].c_str());
            } else if (key=="artist") {
                info->artist = value;
            } else {
                otherItems.push_back(item);
            }
        }
        string otherString = join(otherItems, "\n");
        replace(otherString, "=", ": ");
        info->additionalInfo = wstring(otherString.begin(), otherString.end());
    }
}

bool getMusicInfo(const wchar_t *file, TTrackInfo *info)
{
    FILE *fp = _wfopen(file, L"rb");
    if(!fp)
        return false;

    int header[4];// 4 ints: id, res_area, rom size, crc
    int headerSize = 16;
    fread(&header, 4, 4, fp);
    if(!isValidFile((char*)&header[0]))
    {
        fclose(fp);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    int tagOffset = headerSize + header[1] + header[2];
    if(tagOffset<0 || tagOffset>=fileSize)
    {
        fclose(fp);
        return false;
    }
    fseek(fp, tagOffset, SEEK_SET);
    int sizeNeed = fileSize-tagOffset;
    char *buf = (char*)malloc(sizeNeed+1);
    memset(buf, 0, sizeNeed+1);
    int sizeRead = fread(buf, 1, sizeNeed, fp);
    fclose(fp);
    if(sizeRead != sizeNeed || strncmp(buf, "[TAG]", 5) != 0)
        return false;

    string s = buf+5;
    free(buf);

    fillTrackInfo(s, info);
    return true;
}

bool getMusicInfo(char *data, int dataSize, TTrackInfo *info)
{
    char *pBuf = (char*)data;
    int header[4];// 4 ints: id, res_area, rom size, crc
    int headerSize = 16;
    memcpy(&header, pBuf, headerSize);
    if(!isValidFile((char*)&header[0]))
        return false;

    int fileSize = dataSize;
    int tagOffset = headerSize + header[1] + header[2];
    if(tagOffset <0 || tagOffset>=fileSize)
        return false;

    pBuf += tagOffset;

    if(strncmp(pBuf, "[TAG]", 5) != 0)
        return false;

    string s = pBuf+5;
    fillTrackInfo(s, info);
    return true;
}

TFileParse::TFileParse(wstring file) :
    TAbstractParse(file)
{

}

bool TFileParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    TTrackInfo *trackInfo = new TTrackInfo;
    wstring suffix = extractSuffix(musicInfo->musicFileName);
    if(suffix==L"spu")
        trackInfo->trackName = extractBaseName(musicInfo->musicFileName);
    else if(!getMusicInfo(mFile.c_str(), trackInfo))
        return false;

    musicInfo->musicName = trackInfo->trackName;
    musicInfo->duration = trackInfo->duration;
    musicInfo->additionalInfo = trackInfo->additionalInfo;
    musicInfo->trackList.push_back(trackInfo);

    return true;
}

TTrackInfo *TFileParse::parse(char *data, int size)
{
    TTrackInfo *trackInfo = new TTrackInfo;
    if(!getMusicInfo(data, size, trackInfo))
    {
        delete trackInfo;
        return NULL;
    }
    return trackInfo;
}
