#pragma once
#include "RapiManager.h"
#include <vector>
#include <string>

class FileManager {
private:
    RapiManager* rapi;

public:
    FileManager(RapiManager* r);
    ~FileManager();

    void ListDirectory(const std::wstring& path);
    bool CopyFileToDevice(const std::wstring& localPath, const std::wstring& devicePath);
    bool CopyFileFromDevice(const std::wstring& devicePath, const std::wstring& localPath);
    bool DeleteFileOnDevice(const std::wstring& devicePath);
    bool CreateDirectoryOnDevice(const std::wstring& path);
    bool FileExists(const std::wstring& path);

private:
    std::wstring GetFileTypeString(DWORD fileAttributes);
    std::wstring FormatSize(DWORD fileSizeHigh, DWORD fileSizeLow);
};
