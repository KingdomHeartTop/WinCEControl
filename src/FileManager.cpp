#include "FileManager.h"
#include <iostream>
#include <iomanip>

FileManager::FileManager(RapiManager* r) : rapi(r) {}

FileManager::~FileManager() {}

void FileManager::ListDirectory(const std::wstring& path) {
    std::wcout << L"\n=== Contents of: " << path << L" ===\n" << std::endl;

    WIN32_FIND_DATAW findData = {};
    HANDLE hFind = rapi->CeFindFirstFile((path + L"\\*").c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"[ERROR] Cannot open directory: " << path << L" (Error: " << rapi->CeGetLastError() << L")" << std::endl;
        return;
    }

    std::wcout << L"  [Name]                    [Size]       [Type]" << std::endl;
    std::wcout << L"  " << std::setw(28) << std::left << L"---------------------------" << L" " << std::setw(12) << L"----------" << L" " << L"----------" << std::endl;

    do {
        if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0) {
            continue;
        }

        std::wstring type = GetFileTypeString(findData.dwFileAttributes);
        std::wstring size = FormatSize(findData.nFileSizeHigh, findData.nFileSizeLow);

        std::wcout << L"  " << std::setw(28) << std::left << findData.cFileName
                   << L" " << std::setw(12) << std::right << size
                   << L" " << type << std::endl;

    } while (rapi->CeFindNextFile(hFind, &findData));

    rapi->CeFindClose(hFind);
}

bool FileManager::CopyFileToDevice(const std::wstring& localPath, const std::wstring& devicePath) {
    std::wcout << L"[INFO] Copying " << localPath << L" -> " << devicePath << std::endl;

    HANDLE hLocal = CreateFileW(localPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLocal == INVALID_HANDLE_VALUE) {
        std::wcerr << L"[ERROR] Cannot open local file: " << localPath << std::endl;
        return false;
    }

    HANDLE hDevice = rapi->CeCreateFile(devicePath.c_str(), GENERIC_WRITE, 0, NULL,
                                        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::wcerr << L"[ERROR] Cannot create device file: " << devicePath << std::endl;
        CloseHandle(hLocal);
        return false;
    }

    BYTE buffer[4096];
    DWORD bytesRead = 0, bytesWritten = 0;

    while (ReadFile(hLocal, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        if (!rapi->CeWriteFile(hDevice, buffer, bytesRead, &bytesWritten, NULL) || bytesWritten != bytesRead) {
            std::wcerr << L"[ERROR] Write error during copy" << std::endl;
            break;
        }
    }

    CloseHandle(hLocal);
    rapi->CeCloseHandle(hDevice);

    std::wcout << L"[SUCCESS] File copied successfully" << std::endl;
    return true;
}

bool FileManager::CopyFileFromDevice(const std::wstring& devicePath, const std::wstring& localPath) {
    std::wcout << L"[INFO] Copying " << devicePath << L" -> " << localPath << std::endl;

    HANDLE hDevice = rapi->CeCreateFile(devicePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
                                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::wcerr << L"[ERROR] Cannot open device file: " << devicePath << std::endl;
        return false;
    }

    HANDLE hLocal = CreateFileW(localPath.c_str(), GENERIC_WRITE, 0, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLocal == INVALID_HANDLE_VALUE) {
        std::wcerr << L"[ERROR] Cannot create local file: " << localPath << std::endl;
        rapi->CeCloseHandle(hDevice);
        return false;
    }

    BYTE buffer[4096];
    DWORD bytesRead = 0, bytesWritten = 0;

    while (rapi->CeReadFile(hDevice, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        if (!WriteFile(hLocal, buffer, bytesRead, &bytesWritten, NULL) || bytesWritten != bytesRead) {
            std::wcerr << L"[ERROR] Write error during copy" << std::endl;
            break;
        }
    }

    rapi->CeCloseHandle(hDevice);
    CloseHandle(hLocal);

    std::wcout << L"[SUCCESS] File copied successfully" << std::endl;
    return true;
}

bool FileManager::DeleteFileOnDevice(const std::wstring& devicePath) {
    if (!rapi->CeCreateFile(devicePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) {
        std::wcerr << L"[ERROR] File not found on device: " << devicePath << std::endl;
        return false;
    }

    if (!rapi->CeCreateFile(devicePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) {
        // Используем CeDeleteFile для удаления
        if (!::CeDeleteFile(devicePath.c_str())) { // Добавь в RapiManager обёртку
            std::wcerr << L"[ERROR] Cannot delete file: " << devicePath << std::endl;
            return false;
        }
    }

    std::wcout << L"[SUCCESS] File deleted: " << devicePath << std::endl;
    return true;
}

bool FileManager::CreateDirectoryOnDevice(const std::wstring& path) {
    if (!::CeCreateDirectory(path.c_str(), NULL)) {
        std::wcerr << L"[ERROR] Cannot create directory: " << path << std::endl;
        return false;
    }
    std::wcout << L"[SUCCESS] Directory created: " << path << std::endl;
    return true;
}

bool FileManager::FileExists(const std::wstring& path) {
    WIN32_FIND_DATAW findData = {};
    HANDLE hFind = rapi->CeFindFirstFile(path.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        rapi->CeFindClose(hFind);
        return true;
    }
    return false;
}

std::wstring FileManager::GetFileTypeString(DWORD fileAttributes) {
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return L"<DIR>";
    if (fileAttributes & FILE_ATTRIBUTE_HIDDEN) return L"HIDDEN";
    if (fileAttributes & FILE_ATTRIBUTE_READONLY) return L"READONLY";
    if (fileAttributes & FILE_ATTRIBUTE_SYSTEM) return L"SYSTEM";
    if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE) return L"ARCHIVE";
    return L"FILE";
}

std::wstring FileManager::FormatSize(DWORD fileSizeHigh, DWORD fileSizeLow) {
    ULONGLONG size = ((ULONGLONG)fileSizeHigh << 32) | fileSizeLow;
    if (size < 1024) {
        return std::to_wstring(size) + L" B";
    } else if (size < 1024 * 1024) {
        return std::to_wstring(size / 1024) + L" KB";
    } else if (size < 1024 * 1024 * 1024) {
        return std::to_wstring(size / (1024 * 1024)) + L" MB";
    } else {
        return std::to_wstring(size / (1024 * 1024 * 1024)) + L" GB";
    }
}
