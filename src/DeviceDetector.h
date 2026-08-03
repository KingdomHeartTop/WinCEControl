#pragma once
#include "RapiManager.h"
#include <string>

struct DeviceInfo {
    std::wstring osVersion;
    std::wstring osBuild;
    std::wstring processorType;
    DWORD processorArchitecture;
    DWORD memoryTotal;
    std::wstring deviceName;
};

class DeviceDetector {
private:
    RapiManager* rapi;

public:
    DeviceDetector(RapiManager* r);
    ~DeviceDetector();

    DeviceInfo GetDeviceInfo();
    bool IsDeviceConnected();
    void PrintDeviceInfo();

private:
    std::wstring GetProcessorArchitectureString(WORD architecture);
};
