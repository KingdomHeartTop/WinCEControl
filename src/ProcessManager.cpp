#include "ProcessManager.h"
#include <tlhelp32.h>
#include <iostream>

ProcessManager::ProcessManager(RapiManager* r) : rapi(r) {}

ProcessManager::~ProcessManager() {}

std::vector<ProcessInfo> ProcessManager::ListProcesses() {
    std::vector<ProcessInfo> processes;

    HANDLE hSnapshot = ::CeCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::wcerr << L"[ERROR] Cannot create process snapshot" << std::endl;
        return processes;
    }

    PROCESSENTRY32W pe = {};
    pe.dwSize = sizeof(PROCESSENTRY32W);

    if (::CeProcess32FirstW(hSnapshot, &pe)) {
        do {
            ProcessInfo info;
            info.dwProcessId = pe.th32ProcessID;
            info.name = pe.szExeFile;

            // Попытка получить описание (через модули)
            MODULEENTRY32W me = {};
            me.dwSize = sizeof(MODULEENTRY32W);
            HANDLE hModule = ::CeCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe.th32ProcessID);

            if (hModule != INVALID_HANDLE_VALUE) {
                if (::CeModule32FirstW(hModule, &me)) {
                    info.description = me.szModule;
                }
                ::CeCloseToolhelp32Snapshot(hModule);
            }

            processes.push_back(info);
        } while (::CeProcess32NextW(hSnapshot, &pe));
    }

    ::CeCloseToolhelp32Snapshot(hSnapshot);
    return processes;
}

bool ProcessManager::RunProcess(const std::wstring& exePath, const std::wstring& args) {
    std::wcout << L"[INFO] Starting process: " << exePath << L" " << args << std::endl;

    PROCESS_INFORMATION pi = {};
    STARTUPINFOW si = {};
    si.cb = sizeof(STARTUPINFOW);

    std::wstring cmdLine = L"\"" + exePath + L"\" " + args;

    if (!rapi->CeCreateProcess(NULL, (LPWSTR)cmdLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::wcerr << L"[ERROR] Failed to start process (Error: " << rapi->CeGetLastError() << L")" << std::endl;
        return false;
    }

    std::wcout << L"[SUCCESS] Process started (PID: " << pi.dwProcessId << L")" << std::endl;
    rapi->CeCloseHandle(pi.hProcess);
    rapi->CeCloseHandle(pi.hThread);
    return true;
}

bool ProcessManager::KillProcess(DWORD processId) {
    std::wcout << L"[WARNING] Attempting to terminate process PID=" << processId << std::endl;

    // Сначала пытаемся закрыть окна
    // К сожалению, в CE нет EnumWindows через RAPI, так что используем TerminateProcess

    HANDLE hProcess = ::CeOpenProcess(PROCESS_TERMINATE, FALSE, processId);
    if (hProcess == NULL) {
        std::wcerr << L"[ERROR] Cannot open process" << std::endl;
        return false;
    }

    if (!rapi->CeTerminateProcess(hProcess, 0)) {
        std::wcerr << L"[ERROR] Cannot terminate process" << std::endl;
        ::CeCloseHandle(hProcess);
        return false;
    }

    ::CeCloseHandle(hProcess);
    std::wcout << L"[SUCCESS] Process terminated" << std::endl;
    return true;
}

bool ProcessManager::LaunchInstaller(const std::wstring& cabPath) {
    // wceload.exe - стандартный установщик CAB в Windows CE
    return RunProcess(L"\\Windows\\wceload.exe", L"\"" + cabPath + L"\" /silent");
}
