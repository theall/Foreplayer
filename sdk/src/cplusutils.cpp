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

#include "cplusutils.h"

#include <algorithm>
#include <numeric>

#include <string.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#else
#include <iconv.h>
#include <wctype.h>
#include <wchar.h>
#include <errno.h>
#endif

int wchar2utf8(const wchar_t* szSrc, int nSrcSize, char *szDest, int nDestSize)
{
#ifdef _WIN32
    (void)nSrcSize;
    return WideCharToMultiByte( CP_UTF8, 0, szSrc, -1, szDest, nDestSize, NULL, NULL );
#else
    size_t result;
    iconv_t env;
    env = iconv_open("UTF-8","WCHAR_T");
    if (env==(iconv_t)-1)
    {
        printf("iconv_open WCHAR_T->UTF8 error%s %d/n",strerror(errno),errno) ;
        return -1;
    }
    result = iconv(env,(char**)&szSrc,(size_t*)&nSrcSize,(char**)&szDest,(size_t*)&nDestSize);
    if (result==(size_t)-1)
    {
        printf("iconv WCHAR_T->UTF8 error %d/n",errno) ;
        return -1;
    }
    iconv_close(env);
    return (int)result;
#endif
}

wstring utf82wchar(const char *szSrc, wchar_t *szDest, int nDestSize)
{
#ifdef _WIN32
    (void)szDest;
    (void)nDestSize;
    int bufSize = MultiByteToWideChar( CP_UTF8, 0, szSrc, -1, NULL, -1 ) * sizeof(wchar_t);
    char buf[bufSize];
    MultiByteToWideChar( CP_UTF8, 0, szSrc, -1, (LPWSTR)buf, bufSize );
    return wstring((wchar_t*)buf);
#else
    size_t result;
    iconv_t env;
    int size = strlen(szSrc)+1 ;
    env = iconv_open("WCHAR_T","UTF-8");
    if (env==(iconv_t)-1)
    {
        printf("iconv_open UTF8->WCHAR_T error %d/n",errno) ;
        return -1;
    }
    result = iconv(env,(char**)&szSrc,(size_t*)&size,(char**)&szDest,(size_t*)&nDestSize);
    if (result==(size_t)-1)
    {
        printf("iconv UTF8->WCHAR_T error %d/n",errno) ;
        return -1;
    }
    iconv_close(env);
    return wstring(szDest);
#endif
}

wstring char2wstring(const char *src)
{
    int n = strlen(src) * 2;
    char buf[n];
    return utf82wchar(src, (wchar_t*)buf, n);
}

wstring string2wstring(string s)
{
    setlocale(LC_ALL, "chs");
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

string wstring2string(wstring ws)
{
    string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

wstring extractSuffix(wstring fileName)
{
    int l = fileName.size();
    int pos = fileName.find_last_of(L".");
    if(pos < 0)
        return wstring();
    pos++;
    wstring ret = fileName.substr(pos, l-pos);
    transform(ret.begin(), ret.end(), ret.begin(), ::towlower);
    return ret;
}

wstring extractBaseName(wstring fileName)
{
    int pos = fileName.find_last_of(L"/");
    if(pos < 0)
    {
        pos = fileName.find_last_of(L"\\");
        if(pos < 0)
            return fileName;
    }
    pos++;
    wstring ret = fileName.substr(pos, fileName.size()-pos);
    pos = ret.find_last_of(L".");
    if(pos > 0)
        ret = ret.substr(0, pos);

    return ret;
}

wstring extractPath(wstring fileName)
{
    int pos = fileName.find_last_of(L"/");
    if(pos < 0)
    {
        pos = fileName.find_last_of(L"\\");
        if(pos < 0)
            return fileName;
    }
    pos++;
    wstring ret = fileName.substr(0, pos);
    return ret;
}

void readFile(wstring fileName, char **buffer, long *size)
{
    FILE *fp = _wfopen(fileName.c_str(), L"rb");
    if(fp)
    {
        fseek(fp, 0, SEEK_END);
        int fileSize = ftell(fp);
        if(fileSize > 0)
        {
            *buffer = new char[fileSize];
            *size = fread(*buffer, 1, fileSize, fp);
        }
        fclose(fp);
    }
}

string trim(string s)
{
    if (s.empty())
        return s;

    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

wstring trim(wstring ws)
{
    if (ws.empty())
        return ws;

    ws.erase(0, ws.find_first_not_of(L" "));
    ws.erase(ws.find_last_not_of(L" ") + 1);
    return ws;
}

void split(wstring s, wstring delim, vector<wstring > *ret)
{
    size_t last = 0;
    size_t index = s.find_first_of(delim, last);
    while(index != wstring::npos)
    {
        size_t n = index-last;
        if(n > 0)
            ret->push_back(s.substr(last, index-last));
        last = index+1;
        index = s.find_first_of(delim, last);
    }
    if(index-last > 0)
    {
        ret->push_back(s.substr(last,index-last));
    }
}

wstring lower(wstring s)
{
    transform(s.begin(), s.end(), s.begin(), ::towlower);
    return s;
}

wstring join(vector<wstring> wsl, wstring delim)
{
    return accumulate(wsl.begin() , wsl.end() , delim);
}

string extractSuffix(string fileName)
{
    int l = fileName.size();
    int pos = fileName.find_last_of(".");
    if(pos < 0)
        return string();
    pos++;
    string ret = fileName.substr(pos, l-pos);
    transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}

bool contains(vector<string> sl, string s, bool caseSensitive)
{
    if(!caseSensitive)
        transform(s.begin(), s.end(), s.begin(), ::tolower);

    for(string _s : sl)
    {
        if(!caseSensitive)
            transform(_s.begin(), _s.end(), _s.begin(), ::towlower);

        if(_s == s)
            return true;
    }
    return false;
}

bool contains(vector<wstring> sl, wstring s, bool caseSensitive)
{
    if(!caseSensitive)
        transform(s.begin(), s.end(), s.begin(), ::towlower);

    for(wstring _s : sl)
    {
        if(!caseSensitive)
            transform(_s.begin(), _s.end(), _s.begin(), ::towlower);

        if(_s == s)
            return true;
    }
    return false;
}

const char *extractSuffix(const char *src)
{
    char *p = (char *)src;
    int count = 0;
    while(*p++!='\0')
    {
        if(*p=='.')
            break;
        count++;
        if(count>260)
            return src;
    }
    return p+1;
}

wstring upper(wstring s)
{
    transform(s.begin(), s.end(), s.begin(), ::towupper);
    return s;
}

bool isExist(wstring filePath)
{
    FILE *fp = _wfopen(filePath.c_str(), L"r");
    if(!fp)
        return false;

    fclose(fp);

    return true;
}

wstring getCurrentDir()
{
    wchar_t buf[FILENAME_MAX];
    _wgetcwd(buf, FILENAME_MAX);
    wstring s = buf;
    return s;
}

void setCurrentDirectory(wstring path)
{
    _wchdir(path.c_str());
}

void createDir(wstring path)
{
    _wmkdir(path.c_str());
}

wstring i2wstr(int i)
{
    wchar_t buf[40];
    _itow(i, buf, 10);
    return wstring(buf);
}

wstring readText(wstring filePath)
{
    wstring ret;
    FILE *fp = _wfopen(filePath.c_str(), L"r");
    if(fp)
    {
        fseek(fp, 0, SEEK_END);
        int fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char *buf = new char[fileSize];
        fread(buf, 1, fileSize, fp);
        fclose(fp);

        ret = (wchar_t*)buf;
        delete buf;
    }
    return ret;
}
