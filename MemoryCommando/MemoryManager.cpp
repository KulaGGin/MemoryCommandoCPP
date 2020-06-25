#include "MemoryManager.h"

#include <locale>
#include <Psapi.h>
#include "ProcessAccess.h"
#include "CreateToolhelp32SnapshotException.h"
#include "Process32Exception.h"
#include <wil/resource.h>
#include <boost/locale.hpp>
#include <boost/locale/conversion.hpp>
#include <boost/algorithm/string.hpp>
#include "Module32Exception.h"

#include "External.h"

namespace MemoryCommando::External {
    using namespace Exceptions;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    MemoryManager::MemoryManager(DWORD processId, DWORD processAccess) {
        _processHandle = GetProcessHandle(processId, processAccess);
        _processId = GetProcessId(_processHandle);
        _processName = GetProcessName(_processHandle);
    }

    MemoryManager::MemoryManager(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        _processHandle = GetProcessHandle(processName, processNumber, processAccess);
        _processId = GetProcessId(_processHandle);
        _processName = GetProcessName(_processHandle);
    }

    MemoryManager::MemoryManager(const std::string& processName, size_t processNumber, DWORD processAccess) {
        const std::wstring processNameWide = conv::utf_to_utf<WCHAR>(processName);
        _processHandle = GetProcessHandle(processNameWide, processNumber, processAccess);
        _processId = GetProcessId(_processHandle);
        _processName = GetProcessName(_processHandle);
    }

    std::vector<MODULEENTRY32W> MemoryManager::GetModules() const {
        return External::GetModules(_processId);
    }

    MODULEENTRY32W MemoryManager::GetModule(const std::wstring& moduleName) {
        const auto module = External::GetModule(_processId, moduleName);
        return module;
    }
}
