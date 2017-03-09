#include <windows.h>
#include <shellapi.h>
#include <stdio.h>

#ifdef UNICODE
#define MAIN wWinMain
#else
#define MAIN WinMain
#endif

int WINAPI MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
	int argnum = 1;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD exit_code;
	
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));

	/* Note: argnum is not like argc... */
	if (argnum < 1) {
		fputs("Usage: noconsole <program> [<args>...]\n", stderr);
		ExitProcess(1);
	}

	/* Start the child process */
	if (!CreateProcess(NULL, pCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, /* Note that the previous should maybe be set to the cwd of the program being run */
			&si,
			&pi))
	{
		fprintf(stderr, "Error starting process, code %d.\n", GetLastError());
		ExitProcess(1);
	}

	/* Wait for child exit */
	WaitForSingleObject(pi.hProcess, INFINITE);

	GetExitCodeProcess(pi.hProcess, &exit_code);

	/* Clean up the handles */
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	ExitProcess(exit_code);
	return 0;
}
