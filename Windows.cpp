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
            _T("MemoriaCompartida"));

    if (hMapFile == NULL) {
        _tprintf(_T("No se puede crear el archivo de mapeo (%d).\n"),
                 GetLastError());
        return 1;
    }
    pBuf = (LPTSTR) MapViewOfFile(hMapFile,
                                  FILE_MAP_ALL_ACCESS,
                                  0,
                                  0,
                                  SIZE);
    if (pBuf == NULL) {
        _tprintf(_T("No se puede mapear la vista de archivo (%d).\n"),
                 GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }
    CopyMemory((PVOID)pBuf, _T("¡Hola desde el proceso padre!"),
               (_tcslen(_T("¡Hola desde el proceso padre!")) * sizeof(TCHAR)));

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(_T("Ruta al proceso hijo ejecutable"),
                       NULL,
                       NULL,
                       NULL,
                       FALSE,
                       0,
                       NULL,
                       NULL,
                       &si,
                       &pi))
    {
        printf("Error al crear el proceso(%d).\n", GetLastError());
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}