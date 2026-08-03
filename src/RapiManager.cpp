#include "RapiManager.h"
#include <iostream>

RapiManager::RapiManager() : hRapi(nullptr), connected(false) {}

RapiManager::~RapiManager() {
    Disconnect();
}

bool RapiManager::Connect() {
    if (connected) return true;

    hRapi = CeRapiInit();
    if (hRapi == nullptr) {
        std::cerr << "[ERROR] Failed to initialize RAPI. Device not connected or ActiveSync not running." << std::endl;
        return false;
    }

    connected = true;
    std::cout << "[INFO] Connected to Windows CE device via RAPI" << std::endl;

    // Проверка версии ОС для подтверждения
    OSVERSIONINFOW osvi = {};
    if (CeGetVersionEx(&osvi)) {
        std::wcout << L"[INFO] Device OS: " << osvi.szCSDVersion << L" (v" << osvi.dwMajorVersion << L"." << osvi.dwMinorVersion << L")" << std::endl;
    }

    return true;
}

void RapiManager::Disconnect() {
    if (connected && hRapi != nullptr) {
        CeRapiUninit(hRapi);
        hRapi = nullptr;
        connected = false;
        std::cout << "[INFO] Disconnected from device" << std::endl;
    }
}

BOOL RapiManager::CeGetVersionEx(LPOSVERSIONINFOW lpVersionInfo) {
    if (!connected) return FALSE;
    return ::CeGetVersionEx(lpVersionInfo);
}

BOOL RapiManager::CeGetSystemInfo(LPSYSTEM_INFO lpSystemInfo) {
    if (!connected) return FALSE;
    return ::CeGetSystemInfo(lpSystemInfo);
}

HANDLE RapiManager::CeFindFirstFile(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData) {
    if (!connected) return INVALID_HANDLE_VALUE;
    return ::CeFindFirstFile(lpFileName, lpFindFileData);
}

BOOL RapiManager::CeFindNextFile(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData) {
    if (!connected) return FALSE;
    return ::CeFindNextFile(hFindFile, lpFindFileData);
}

BOOL RapiManager::CeFindClose(HANDLE hFindFile) {
    if (!connected) return FALSE;
    return ::CeFindClose(hFindFile);
}

HANDLE RapiManager::CeCreateFile(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                                 LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
                                 DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    if (!connected) return INVALID_HANDLE_VALUE;
    return ::CeCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
                          dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL RapiManager::CeReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
                             LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) {
    if (!connected) return FALSE;
    return ::CeReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

BOOL RapiManager::CeWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
                              LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
    if (!connected) return FALSE;
    return ::CeWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

BOOL RapiManager::CeCloseHandle(HANDLE hObject) {
    if (!connected) return FALSE;
    return ::CeCloseHandle(hObject);
}

BOOL RapiManager::CeCreateProcess(LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
                                  LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
                                  BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment,
                                  LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo,
                                  LPPROCESS_INFORMATION lpProcessInformation) {
    if (!connected) return FALSE;
    return ::CeCreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
                             bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory,
                             lpStartupInfo, lpProcessInformation);
}

BOOL RapiManager::CeTerminateProcess(HANDLE hProcess, UINT uExitCode) {
    if (!connected) return FALSE;
    return ::CeTerminateProcess(hProcess, uExitCode);
}

DWORD RapiManager::CeGetLastError() {
    if (!connected) return ERROR_NOT_CONNECTED;
    return ::CeGetLastError();
}
