#include "MemoryManagerExternal.h"

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

#include "SharedMethods.h"

namespace MemoryCommanderCpp {
    using namespace Exceptions;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    MemoryManagerExternal::MemoryManagerExternal(DWORD processId, DWORD processAccess) {
        _processHandle = OpenProcess(processId, processAccess);
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        _processHandle = OpenProcess(processName, processNumber, processAccess);
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::string& processName, size_t processNumber, DWORD processAccess) {
        const std::wstring processNameWide = conv::utf_to_utf<WCHAR>(processName);
        _processHandle = OpenProcess(processNameWide, processNumber, processAccess);
    }

    std::vector<MODULEENTRY32W> MemoryManagerExternal::GetModules() const {
        const DWORD processId = ::GetProcessId(_processHandle);
        return MemoryCommanderCpp::GetModules(processId);
    }

    std::vector<HMODULE> MemoryManagerExternal::GetModulesHandles() const {
        return MemoryCommanderCpp::GetModulesHandles(_processHandle);
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
