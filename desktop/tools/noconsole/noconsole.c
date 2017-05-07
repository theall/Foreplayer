/*

  noconsole --- run console application without console window ---

  Copyright (C) 2016 Masamichi Hosoda. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.

 */

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

static TCHAR szAppName[] = TEXT ("noconsole");

static void MessageBoxFormatMessage (LPCTSTR szMessage)
{
  LPVOID lpBuffer;
  TCHAR m[1024] = { 0 };

  if (FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
                     FORMAT_MESSAGE_FROM_SYSTEM,
                     NULL, GetLastError (),
                     MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
                     (LPTSTR) &lpBuffer, 0, NULL))
    {
      if (SUCCEEDED (StringCbPrintf (m, sizeof (m), TEXT ("%s: %s"),
                                     szMessage, (LPTSTR) lpBuffer)))
        MessageBox (NULL, m, szAppName, MB_OK);
      else
        MessageBox (NULL, (LPTSTR) lpBuffer, szAppName, MB_OK);
      LocalFree (lpBuffer);
    }
  else
    {
      if (SUCCEEDED (StringCbPrintf (m, sizeof (m), TEXT ("%s: failed"),
                                     szMessage)))
        MessageBox (NULL, m, szAppName, MB_OK);
      else
        MessageBox (NULL, TEXT ("failed"), szAppName, MB_OK);
    }
}

int WINAPI _tWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine, int nShowCmd)
{
  STARTUPINFO si = { sizeof (STARTUPINFO) };
  PROCESS_INFORMATION pi = {};
  DWORD result;

  if (!CreateProcess(NULL, lpCmdLine, NULL, NULL, FALSE,
                     CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {
      MessageBoxFormatMessage (TEXT ("CreateProcess"));
      return -1;
    }

  if (!CloseHandle (pi.hThread))
    MessageBoxFormatMessage (TEXT ("CloseHandle (Thread)"));

  result = WaitForSingleObject (pi.hProcess, INFINITE);
  switch (result)
    {
    case WAIT_ABANDONED:
      MessageBox (NULL, TEXT ("WAIT_ABANDONED"), szAppName, MB_OK);
      break;
    case WAIT_OBJECT_0:
      {
        DWORD ecode;
        if (!GetExitCodeProcess (pi.hProcess, &ecode))
          MessageBoxFormatMessage (TEXT ("GetExitCodeProcess"));
        else
          {
            if (!CloseHandle (pi.hProcess))
              MessageBoxFormatMessage (TEXT ("CloseHandle (Process)"));
            return ecode;
          }
      }
      break;
    case WAIT_TIMEOUT:
      MessageBox (NULL, TEXT ("WAIT_TIMEOUT"), szAppName, MB_OK);
      break;
    case WAIT_FAILED:
      MessageBoxFormatMessage (TEXT ("WaitForSingleObject WAIT_FAILED"));
      break;
    default:
      {
        TCHAR m[1024];

        if (SUCCEEDED (StringCbPrintf (m, sizeof (m),
                                       TEXT ("WaitForSingleObject: "
                                             "unknown return value 0x%08x")
                                       , result)))
          MessageBox (NULL, m, szAppName, MB_OK);
        else
          MessageBox (NULL, TEXT ("WaitForSingleObject: failed"),
                      szAppName, MB_OK);
      }
      break;
    }

  if (!CloseHandle (pi.hProcess))
    MessageBoxFormatMessage (TEXT ("CloseHandle (Process)"));

  return -1;
}
