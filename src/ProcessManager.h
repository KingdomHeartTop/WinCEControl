#pragma once
#include "RapiManager.h"
#include <vector>

struct ProcessInfo {
    DWORD dwProcessId;
    std::wstring name;
    std::wstring description;
};

class ProcessManager {
private:
    RapiManager* rapi;

public:
    ProcessManager(RapiManager* r);
    ~ProcessManager();

    std::vector<ProcessInfo> ListProcesses();
    bool RunProcess(const std::wstring& exePath, const std::wstring& args = L"");
    bool KillProcess(DWORD processId);
    bool LaunchInstaller(const std::wstring& cabPath);
};
