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

}