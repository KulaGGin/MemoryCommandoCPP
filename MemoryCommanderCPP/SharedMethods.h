#pragma once
#include <windows.h>

#include <boost/locale/encoding_utf.hpp>
#include <string>

#include <TlHelp32.h>
#include <wil/resource.h>
#include <boost/algorithm/string.hpp>

#include "CreateToolhelp32SnapshotException.h"
#include "Process32Exception.h"

#include <string>

#include "Module32Exception.h"

namespace MemoryCommanderCpp {
    using namespace Exceptions;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;


    //std::vector<PROCESSENTRY32W> GetRunningProcesses() {
    //}

    DWORD GetProcessId(const std::wstring& processName, const size_t processNumber);

    std::vector<MODULEENTRY32W> GetModules(const DWORD& processId);

    HANDLE OpenProcess(DWORD processId, DWORD processAccess);
    HANDLE OpenProcess(const std::wstring& processName, size_t processNumber, DWORD processAccess);

}
