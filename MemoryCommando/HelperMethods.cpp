#include "HelperMethods.h"



#include <sstream>
#include <windows.h>
#include <stdexcept>
#include <wil/resource.h>

#include "Exceptions/CreateToolhelp32SnapshotException.h"
#include "Exceptions/GetProcessIdException.h"
#include "Exceptions/Module32Exception.h"
#include "Exceptions/OpenProcessException.h"
#include "Exceptions/Process32Exception.h"

namespace MemoryCommando {
    SYSTEM_INFO HelperMethods::GetSystemInfo() {
        SYSTEM_INFO systemInfo;
        ::GetNativeSystemInfo(&systemInfo);
        return systemInfo;
    }

    std::vector<PROCESSENTRY32W> HelperMethods::GetRunningProcesses() {
        std::vector<PROCESSENTRY32W> runningProcesses{};
        PROCESSENTRY32 process{};
        process.dwSize = sizeof(process);

        const wil::unique_tool_help_snapshot processesSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

        if(!processesSnapshot)
            throw Exceptions::CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        bool copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if(!copiedToBuffer)
            throw Exceptions::Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        do {
            runningProcesses.push_back(process);
            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        } while(copiedToBuffer);

        if(!copiedToBuffer && GetLastError() != ERROR_NO_MORE_FILES)
            throw Exceptions::Process32Exception("Process32Next failed to fill the buffer.", GetLastError());

        return runningProcesses;
    }

    PROCESSENTRY32W HelperMethods::GetProcess(const DWORD processId) {
        std::vector<PROCESSENTRY32W> processes = GetRunningProcesses();

        for(auto currentProcess : processes) {
            if(processId == currentProcess.th32ProcessID)
                return currentProcess;
        }

        throw std::runtime_error("Couldn't find a process with the specified processId in the process list.");
    }

    PROCESSENTRY32W HelperMethods::GetProcess(const HANDLE processHandle) {
        const DWORD processId = GetProcessId(processHandle);
        const PROCESSENTRY32W process = GetProcess(processId);

        return process;
    }

    PROCESSENTRY32W HelperMethods::GetProcess(const std::wstring& processName, const size_t processNumber) {
        auto processes = GetRunningProcesses();

        size_t foundProcessNumber = 0;
        for(auto currentProcess : processes) {
            if(processName.compare(std::wstring(currentProcess.szExeFile)) == 0) {
                foundProcessNumber++;

                if(foundProcessNumber >= processNumber) {
                    return currentProcess;
                }
            }
        }

        throw std::runtime_error("Couldn't find a process with the specified process name in the process list.");
    }

    DWORD HelperMethods::GetProcessId(const HANDLE processHandle) {
        const DWORD processId = ::GetProcessId(processHandle);

        if(!processId)
            throw Exceptions::GetProcessIdException("Couldn't get processId from the specified processHandle", GetLastError());

        return processId;
    }

    DWORD HelperMethods::GetProcessId(const std::wstring& processName, const size_t processNumber) {
        const PROCESSENTRY32W process = GetProcess(processName, processNumber);
        const DWORD processId = process.th32ProcessID;

        return processId;
    }

    HANDLE HelperMethods::GetProcessHandle(const DWORD processId, const DWORD processAccess) {
        const HANDLE processHandle = ::OpenProcess(processAccess, 0, processId);

        if(!processHandle)
            throw Exceptions::OpenProcessException("OpenProcess couldn't get a handle to the specified processId with the error " + std::to_string(GetLastError()) + ".", GetLastError());

        return processHandle;
    }

    HANDLE HelperMethods::GetProcessHandle(const std::wstring& processName, const size_t processNumber, const DWORD processAccess) {
        const PROCESSENTRY32W process = GetProcess(processName, processNumber);
        const HANDLE processHandle = GetProcessHandle(process.th32ProcessID, processAccess);

        return processHandle;
    }

    std::wstring HelperMethods::GetProcessName(const HANDLE processHandle) {
        const auto process = GetProcess(processHandle);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::wstring HelperMethods::GetProcessName(DWORD processId) {
        const auto process = GetProcess(processId);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::vector<MODULEENTRY32W> HelperMethods::GetModules(const DWORD processId) {
        std::vector<MODULEENTRY32W> modules{};
        MODULEENTRY32               module{};
        module.dwSize = sizeof(module);

        const auto modulesSnapshot = wil::unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));

        if(!modulesSnapshot)
            throw Exceptions::CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot of modules.", GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if(!copiedToBuffer)
            throw Exceptions::Module32Exception("Module32First failed to fill the buffer.", GetLastError());

        do {
            modules.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        } while(copiedToBuffer);

        if(GetLastError() != ERROR_NO_MORE_FILES)
            throw Exceptions::Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modules;
    }

    std::vector<MODULEENTRY32W> HelperMethods::GetModules(HANDLE processHandle) {
        const auto processId = GetProcessId(processHandle);
        const auto modules = GetModules(processId);

        return modules;
    }

    std::vector<MODULEENTRY32W> HelperMethods::GetModules(const std::wstring& processName, size_t processNumber) {
        const auto processId = GetProcessId(processName, processNumber);
        const auto modules = GetModules(processId);

        return modules;
    }

    MODULEENTRY32W HelperMethods::GetModule(const std::wstring& moduleName, DWORD processId) {
        std::vector<MODULEENTRY32W> modules = GetModules(processId);

        for(auto currentModule : modules) {
            if(moduleName.compare(std::wstring(currentModule.szModule)) == 0)
                return currentModule;
        }

        throw std::runtime_error("Couldn't find a module with the specified name in the modules list.");
    }

    MODULEENTRY32W HelperMethods::GetModule(const std::wstring& moduleName, HANDLE processHandle) {
        const auto processId = GetProcessId(processHandle);
        const auto module = GetModule(moduleName, processId);

        return module;
    }

    MODULEENTRY32W HelperMethods::GetModule(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto processId = GetProcessId(processName, processNumber);
        const auto module = GetModule(moduleName, processId);

        return module;
    }

    uintptr_t HelperMethods::GetModuleBaseAddress(const std::wstring& moduleName, DWORD processId) {
        auto module = GetModule(moduleName, processId);
        return uintptr_t(module.modBaseAddr);
    }

    uintptr_t HelperMethods::GetModuleBaseAddress(const std::wstring& moduleName, HANDLE processHandle) {
        const auto module = GetModule(moduleName, processHandle);
        return uintptr_t(module.modBaseAddr);
    }

    uintptr_t HelperMethods::GetModuleBaseAddress(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto module = GetModule(moduleName, processName, processNumber);
        return uintptr_t(module.modBaseAddr);
    }

    size_t HelperMethods::GetModuleSize(const std::wstring& moduleName, DWORD processId) {
        const auto module = GetModule(moduleName, processId);
        return size_t(module.modBaseSize);
    }

    size_t HelperMethods::GetModuleSize(const std::wstring& moduleName, HANDLE processHandle) {
        const auto module = GetModule(moduleName, processHandle);
        return size_t(module.modBaseSize);
    }

    size_t HelperMethods::GetModuleSize(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto module = GetModule(moduleName, processName, processNumber);
        return size_t(module.modBaseSize);
    }

    std::vector<std::string> HelperMethods::SplitString(const std::string& string, const char delimiter) {
        std::vector<std::string> tokens;
        std::string              token;
        std::istringstream       tokenStream(string);

        while(std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }
}
