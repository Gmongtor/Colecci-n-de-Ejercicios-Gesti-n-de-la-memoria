#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#define SIZE 4096

int _tmain() {
    HANDLE hMapFile;
    LPCTSTR pBuf;


    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            SIZE,
            _T("SharedMemory"));

    if (hMapFile == NULL) {
        _tprintf(_T("Could not create file mapping object (%d).\n"),
                 GetLastError());
        return 1;
    }
    pBuf = (LPTSTR) MapViewOfFile(hMapFile,
                                  FILE_MAP_ALL_ACCESS,
                                  0,
                                  0,
                                  SIZE);
    if (pBuf == NULL) {
        _tprintf(_T("Could not map view of file (%d).\n"),
                 GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }
    CopyMemory((PVOID)pBuf, _T("Hello from the parent process!"),
               (_tcslen(_T("Hello from the parent process!")) * sizeof(TCHAR)));

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

}