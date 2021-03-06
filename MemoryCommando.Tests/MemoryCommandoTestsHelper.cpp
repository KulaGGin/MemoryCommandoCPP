#include "MemoryCommandoTestsHelper.h"


#include <codecvt>
#include <Psapi.h>
#include <Shlobj.h>
#include <Shlobj_core.h>

namespace MemoryCommandoTests {

    MemoryCommandoTestsHelper::MemoryCommandoTestsHelper() :
        _currentProcessId(InitializeCurrentProcessId()),
        _currentProcessHandle(InitializeCurrentProcessHandle()),
        _currentProcessName(InitializeCurrentProcessName()),
        _testedProcessInformation{ InitializeTestedProcess() },
        _memoryManagerExternal{ std::make_shared<MemoryManagerExternal>(_testedProcessInformation.dwProcessId) } {

    }

    MemoryCommandoTestsHelper::~MemoryCommandoTestsHelper() {
        TerminateProcess(_testedProcessInformation.hProcess, 0);
        // Close process and thread handles. 
        CloseHandle(_testedProcessInformation.hProcess);
        CloseHandle(_testedProcessInformation.hThread);
    }

    DWORD MemoryCommandoTestsHelper::GetCurrentProcessId() const {
        return _currentProcessId;
    }

    wil::unique_handle& MemoryCommandoTestsHelper::GetsCurrentProcessHandle() {
        return _currentProcessHandle;
    }

    std::wstring MemoryCommandoTestsHelper::GetCurrentProcessName() const {
        return  _currentProcessName;
    }

    PROCESS_INFORMATION MemoryCommandoTestsHelper::GetTestedProcess() {
        return _testedProcessInformation;
    }

    std::wstring MemoryCommandoTestsHelper::GetSystemDirectoryPath() const {
        const auto systemDirectoryPathBuffer = std::make_unique<CHAR[]>(MAX_PATH);

        BOOL isWow64Process{};
        IsWow64Process(_currentProcessHandle.get(), &isWow64Process);

        int csidl;
        if(isWow64Process)
            csidl = CSIDL_SYSTEMX86;
        else
            csidl = CSIDL_SYSTEM;

        SHGetSpecialFolderPathA(nullptr, LPSTR(systemDirectoryPathBuffer.get()), csidl, false);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> stringToWStringConverter;
        std::wstring systemDirectoryPath = stringToWStringConverter.from_bytes(systemDirectoryPathBuffer.get());

        return systemDirectoryPath;
    }

    std::shared_ptr<const MemoryManagerExternal> MemoryCommandoTestsHelper::GetMemoryManagerExternal() const {
        return _memoryManagerExternal;
    }

    MemoryManagerInternal& MemoryCommandoTestsHelper::GetMemoryManagerInternal() {
        return _memoryManagerInternal;
    };

    // ReSharper disable once CppMemberFunctionMayBeStatic
    DWORD MemoryCommandoTestsHelper::InitializeCurrentProcessId() const {
        return ::GetCurrentProcessId();
    }

    wil::unique_handle MemoryCommandoTestsHelper::InitializeCurrentProcessHandle() const {
        wil::unique_handle uniqueHandle{ OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId) };
        return uniqueHandle;
    }

    std::wstring MemoryCommandoTestsHelper::InitializeCurrentProcessName() const {
        const auto fileNamePointer = std::make_unique<CHAR[]>(MAX_PATH);

        GetModuleBaseNameA(_currentProcessHandle.get(), nullptr, fileNamePointer.get(), MAX_PATH * sizeof(CHAR));
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> stringToWStringConverter;
        std::wstring processName = stringToWStringConverter.from_bytes(fileNamePointer.get());

        return processName;
    }

    PROCESS_INFORMATION MemoryCommandoTestsHelper::InitializeTestedProcess() const {
        STARTUPINFO startupInformation{};
        startupInformation.dwFlags = STARTF_USESHOWWINDOW;
        startupInformation.wShowWindow = SW_HIDE;
        PROCESS_INFORMATION processInformation;

        const std::wstring notepadPath = GetSystemDirectoryPath() + L"\\notepad.exe";


        CreateProcess(notepadPath.c_str(),                 // Name of program to execute
                      NULL,                                // Command line
                      NULL,                                // Process handle not inheritable
                      NULL,                                // Thread handle not inheritable
                      FALSE,                               // Set handle inheritance to FALSE
                      CREATE_NO_WINDOW | DETACHED_PROCESS, // No creation flags
                      NULL,                                // Use parent's environment block
                      NULL,                                // Use parent's starting directory 
                      &startupInformation,                 // Pointer to STARTUPINFO structure
                      &processInformation);                // Pointer to PROCESS_INFORMATION structure

        return processInformation;
    }
}
