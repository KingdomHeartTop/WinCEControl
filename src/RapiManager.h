#pragma once
#include <windows.h>
#include <rapi.h>

class RapiManager {
private:
    HANDLE hRapi;
    bool connected;

public:
    RapiManager();
    ~RapiManager();

    bool Connect();
    void Disconnect();
    bool IsConnected() const { return connected; }

    // Обёртки для RAPI-функций
    BOOL CeGetVersionEx(LPOSVERSIONINFOW lpVersionInfo);
    BOOL CeGetSystemInfo(LPSYSTEM_INFO lpSystemInfo);
    HANDLE CeFindFirstFile(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
    BOOL CeFindNextFile(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
    BOOL CeFindClose(HANDLE hFindFile);
    HANDLE CeCreateFile(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                        LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                        DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
    BOOL CeReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
    BOOL CeWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
    BOOL CeCloseHandle(HANDLE hObject);
    BOOL CeCreateProcess(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes,
                         LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags,
                         LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo,
                         LPPROCESS_INFORMATION lpProcessInformation);
    BOOL CeTerminateProcess(HANDLE hProcess, UINT uExitCode);
    DWORD CeGetLastError();
};
