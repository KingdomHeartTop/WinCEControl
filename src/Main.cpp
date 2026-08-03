#include "RapiManager.h"
#include "FileManager.h"
#include "ProcessManager.h"
#include "DeviceDetector.h"
#include <iostream>
#include <string>
#include <sstream>

void PrintMenu() {
    std::wcout << L"\n========================================" << std::endl;
    std::wcout << L"        WinCEControl v1.0" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"  1. Device Info" << std::endl;
    std::wcout << L"  2. List Directory" << std::endl;
    std::wcout << L"  3. Copy File (PC -> Device)" << std::endl;
    std::wcout << L"  4. Copy File (Device -> PC)" << std::endl;
    std::wcout << L"  5. Delete File on Device" << std::endl;
    std::wcout << L"  6. List Running Processes" << std::endl;
    std::wcout << L"  7. Run Process" << std::endl;
    std::wcout << L"  8. Kill Process" << std::endl;
    std::wcout << L"  9. Install CAB Package" << std::endl;
    std::wcout << L"  10. Exit" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"  Choose: ";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::wcout.imbue(std::locale(""));

    std::wcout << L"\n=== WinCEControl: Universal Windows CE Manager ===" << std::endl;

    RapiManager rapi;
    FileManager file(&rapi);
    ProcessManager process(&rapi);
    DeviceDetector detector(&rapi);

    // Подключение
    if (!rapi.Connect()) {
        std::wcerr << L"[FATAL] Cannot connect to any CE device. Please check ActiveSync/WMDC." << std::endl;
        system("pause");
        return 1;
    }

    int choice = 0;
    std::wstring input;

    while (true) {
        PrintMenu();
        std::getline(std::wcin, input);

        if (input.empty()) continue;

        std::wstringstream ss(input);
        ss >> choice;

        switch (choice) {
            case 1: {
                detector.PrintDeviceInfo();
                break;
            }

            case 2: {
                std::wcout << L"Enter directory path (e.g., \\Windows): ";
                std::wstring path;
                std::getline(std::wcin, path);
                if (path.empty()) path = L"\\";
                file.ListDirectory(path);
                break;
            }

            case 3: {
                std::wcout << L"Local file path (PC): ";
                std::wstring localPath;
                std::getline(std::wcin, localPath);

                std::wcout << L"Device destination path: ";
                std::wstring devPath;
                std::getline(std::wcin, devPath);

                file.CopyFileToDevice(localPath, devPath);
                break;
            }

            case 4: {
                std::wcout << L"Device source path: ";
                std::wstring devPath;
                std::getline(std::wcin, devPath);

                std::wcout << L"Local destination path (PC): ";
                std::wstring localPath;
                std::getline(std::wcin, localPath);

                file.CopyFileFromDevice(devPath, localPath);
                break;
            }

            case 5: {
                std::wcout << L"Device file path to delete: ";
                std::wstring path;
                std::getline(std::wcin, path);
                file.DeleteFileOnDevice(path);
                break;
            }

            case 6: {
                auto procs = process.ListProcesses();
                std::wcout << L"\n=== Running Processes ===" << std::endl;
                for (const auto& p : procs) {
                    std::wcout << L"  PID=" << p.dwProcessId
                               << L"  Name=" << p.name
                               << L"  (" << p.description << L")" << std::endl;
                }
                std::wcout << L"Total: " << procs.size() << L" processes" << std::endl;
                break;
            }

            case 7: {
                std::wcout << L"Executable path on device (e.g., \\Windows\\Notepad.exe): ";
                std::wstring exePath;
                std::getline(std::wcin, exePath);

                std::wcout << L"Arguments (optional): ";
                std::wstring args;
                std::getline(std::wcin, args);

                process.RunProcess(exePath, args);
                break;
            }

            case 8: {
                std::wcout << L"Process ID to kill: ";
                DWORD pid;
                std::wcin >> pid;
                std::wcin.ignore();
                process.KillProcess(pid);
                break;
            }

            case 9: {
                std::wcout << L"CAB file path on device: ";
                std::wstring cabPath;
                std::getline(std::wcin, cabPath);
                process.LaunchInstaller(cabPath);
                break;
            }

            case 10:
                std::wcout << L"Exiting..." << std::endl;
                rapi.Disconnect();
                return 0;

            default:
                std::wcout << L"[ERROR] Invalid choice!" << std::endl;
        }
    }

    return 0;
}
