#pragma once
#include <vector>
#include <windows.h>

#include <tlhelp32.h>


namespace MemoryCommando::Memory {
    SYSTEM_INFO GetSystemInfo();
    std::vector<PROCESSENTRY32W> GetRunningProcesses();
}

