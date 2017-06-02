#include <iostream>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
using namespace std;

int pushFile(char *fileName)
{
    int i;
    int pos;
    BOOL bMore;
    HANDLE hSnap;
    MODULEENTRY32 me;
    DWORD opt;
    PROCESS_INFORMATION pInfo;
    STARTUPINFOA startInfo;
    DEBUG_EVENT devent;
    char buffer[MAX_PATH];
    ZeroMemory(&me, sizeof(MODULEENTRY32));
    me.dwSize = sizeof(MODULEENTRY32);
    ZeroMemory(&pInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&startInfo, sizeof(STARTUPINFO));
    startInfo.dwFlags   =   STARTF_USESHOWWINDOW;
    startInfo.wShowWindow   =   SW_SHOW;
    startInfo.cb = sizeof(STARTUPINFO);
    opt = DETACHED_PROCESS;
    if (!CreateProcess(
        fileName,
        NULL,
        NULL,
        NULL,
        false,
        opt,
        NULL,
        NULL,
        &startInfo,
        &pInfo))
    {
        printf("Failed to create process, %d\n", (int)GetLastError());
        return -1;
    }
    bool state = true;
//    while(state)
//    {
//        if(WaitForDebugEvent(&devent, INFINITE))             //等待调试事件
//        {
//            switch(devent.dwDebugEventCode)
//            {
//            case EXIT_PROCESS_DEBUG_EVENT:
//                return 0;
//                break;
//            case EXCEPTION_DEBUG_EVENT:
//                //GetModuleFileName((HMODULE)devent.u.LoadDll.lpBaseOfDll, modName, MAX_PATH);
////                if (devent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
////                    state = false;
//                break;
//            case CREATE_THREAD_DEBUG_EVENT:
//                //GetModuleFileName((HMODULE)devent.u.LoadDll.lpBaseOfDll, modName, MAX_PATH);
//                state = false;
//                break;
//            case LOAD_DLL_DEBUG_EVENT:
//               //GetModuleFileName((HMODULE)devent.u.LoadDll.lpBaseOfDll, modName,  MAX_PATH);
//                break;
//            case EXCEPTION_BREAKPOINT:
//                //state = false;
//                break;
//            case CREATE_PROCESS_DEBUG_EVENT:
//                break;
//            }
//            ContinueDebugEvent(devent.dwProcessId,  devent.dwThreadId, DBG_CONTINUE);
//        }
//    }
    bMore = false;
    Sleep(3000);
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pInfo.dwProcessId);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        printf("Failed to create processes snapshot, %d\n", (int)GetLastError());
        return -1;
    }
    vector<string> fileList;
    vector<string> fileNameList;
    GetWindowsDirectory(buffer, MAX_PATH);
    string systemDir = buffer;
    GetCurrentDirectory(MAX_PATH, buffer);
    string currentDir = buffer;
    bMore = Module32First(hSnap, &me);
    while(bMore)
    {
        string fileNameTemp = me.szExePath;
        if(fileNameTemp.substr(0, systemDir.size())!=systemDir &&
                fileNameTemp.substr(0, currentDir.size())!=currentDir)
        {
            //printf("%s\n", fileNameTemp.c_str());
            fileList.push_back(fileNameTemp);
            pos = fileNameTemp.find_last_of('\\');
            fileNameTemp = me.szExePath + pos + 1;
            fileNameList.push_back(fileNameTemp);
        }
        bMore = Module32Next(hSnap, &me);
    }
    for (i=0;i<(int)fileList.size();i++)
    {
        string tmp;
        tmp = currentDir + "\\";
        tmp += fileNameList[i];
        printf("Copying file %s\n", fileList[i].c_str());
        CopyFile((char*)fileList[i].c_str(), (char*)tmp.c_str(), true);
    }
    CloseHandle(hSnap);
    TerminateProcess(pInfo.hProcess, 0);
    return 0;
}

int printUsage()
{
    printf("Copy dependency libraries to current directory.\n");
    printf("Copyright (C) Bilge Theall, all rights reserved.\n");
    printf("\nUsage:\n");
    printf("\tCopyDependency program\n");
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 2)
        return printUsage();

    return pushFile(argv[1]);
}
