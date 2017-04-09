#include "moddir.h"

#ifdef __WIN32__
    #include <windows.h>
#endif

#define MODULE_NAME     L"m1.dll"

wchar_t szFilePath[280];

wchar_t *currentModulePath()
{
#ifdef __WIN32__
    GetModuleFileNameW(GetModuleHandleW(MODULE_NAME), (LPWSTR)szFilePath, sizeof(szFilePath));
    wchar_t *p = szFilePath + lstrlenW(szFilePath) - lstrlenW(MODULE_NAME);
    *p = 0;
#endif

    return szFilePath;
}

wchar_t *c2w(char *str)
{
    wchar_t* wp;
#ifdef __WIN32__
    int bufSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    wp = new wchar_t[bufSize];
    MultiByteToWideChar(CP_ACP, 0, str, -1, wp, bufSize);
#endif
    return wp;
}
