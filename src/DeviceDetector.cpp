#include "DeviceDetector.h"
#include <iostream>
#include <iomanip>

DeviceDetector::DeviceDetector(RapiManager* r) : rapi(r) {}

DeviceDetector::~DeviceDetector() {}

bool DeviceDetector::IsDeviceConnected() {
    return rapi->IsConnected();
}

DeviceInfo DeviceDetector::GetDeviceInfo() {
    DeviceInfo info = {};

    if (!rapi->IsConnected()) {
        return info;
    }

    // Версия ОС
    OSVERSIONINFOW osvi = {};
    if (rapi->CeGetVersionEx(&osvi)) {
        info.osVersion = std::to_wstring(osvi.dwMajorVersion) + L"." +
                         std::to_wstring(osvi.dwMinorVersion);
        info.osBuild = std::to_wstring(osvi.dwBuildNumber);
    }

    // Системная информация
    SYSTEM_INFO si = {};
    rapi->CeGetSystemInfo(&si);
    info.processorArchitecture = si.dwProcessorType;
    info.processorType = GetProcessorArchitectureString(si.wProcessorArchitecture);

    // Память (приблизительно через файл)
    MEMORYSTATUS mem = {};
    mem.dwLength = sizeof(MEMORYSTATUS);
    if (::CeGlobalMemoryStatus(&mem)) {
        info.memoryTotal = mem.dwTotalPhys / (1024 * 1024); // MB
    }

    // Имя устройства (через реестр)
    // Не реализовано для краткости

    return info;
}

void DeviceDetector::PrintDeviceInfo() {
    if (!rapi->IsConnected()) {
        std::wcout << L"[ERROR] Device not connected" << std::endl;
        return;
    }

    DeviceInfo info = GetDeviceInfo();

    std::wcout << L"\n========================================" << std::endl;
    std::wcout << L"     DEVICE INFORMATION" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"  OS Version:      " << info.osVersion << L" (Build " << info.osBuild << L")" << std::endl;
    std::wcout << L"  Processor:       " << info.processorType << std::endl;
    std::wcout << L"  Architecture:    " << std::hex << info.processorArchitecture << std::dec << std::endl;
    std::wcout << L"  Total RAM:       " << info.memoryTotal << L" MB" << std::endl;
    std::wcout << L"========================================\n" << std::endl;
}

std::wstring DeviceDetector::GetProcessorArchitectureString(WORD architecture) {
    switch (architecture) {
        case PROCESSOR_ARCHITECTURE_ARM:   return L"ARM";
        case PROCESSOR_ARCHITECTURE_MIPS:  return L"MIPS";
        case PROCESSOR_ARCHITECTURE_SHX:   return L"SH4";
        case PROCESSOR_ARCHITECTURE_INTEL: return L"x86";
        default: return L"Unknown";
    }
}
