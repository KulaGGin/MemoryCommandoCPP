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

namespace MemoryCommando {
    using namespace Exceptions;
    using namespace External;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    MemoryManager::MemoryManager(DWORD processId, DWORD processAccess) {
        _processHandle = OpenProcess(processId, processAccess);
    }

    MemoryManager::MemoryManager(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        _processHandle = OpenProcess(processName, processNumber, processAccess);
    }

    MemoryManager::MemoryManager(const std::string& processName, size_t processNumber, DWORD processAccess) {
        const std::wstring processNameWide = conv::utf_to_utf<WCHAR>(processName);
        _processHandle = OpenProcess(processNameWide, processNumber, processAccess);
    }

    std::vector<MODULEENTRY32W> MemoryManager::GetModules() const {
        const DWORD processId = ::GetProcessId(_processHandle);
        return MemoryCommando::GetModules(processId);
    }

    std::vector<HMODULE> MemoryManager::GetModulesHandles() const {
        return MemoryCommando::GetModulesHandles(_processHandle);
    }

    //HMODULE MemoryManagerExternal::GetModule(wstring moduleName) {
    //    HMODULE module;
    //    vector<HMODULE> modules = GetModulesHandles();
    //    for(auto module : modules) {
    //        module.name
    //    }

    //    return module;
    //}
}
