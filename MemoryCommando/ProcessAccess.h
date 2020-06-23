#pragma once

namespace MemoryCommanderCpp {
    enum ProcessAccess : unsigned long {
        ProcessTerminate = PROCESS_TERMINATE,
        ProcessCreateThread = PROCESS_CREATE_THREAD,
        ProcessSetSessionId = PROCESS_SET_SESSIONID,
        ProcessVMOperation = PROCESS_VM_OPERATION,
        ProcessVMRead = PROCESS_VM_READ,
        ProcessVMWrite = PROCESS_VM_WRITE,
        ProcessDupHandle = PROCESS_DUP_HANDLE,
        ProcessCreateProcess = PROCESS_CREATE_PROCESS,
        ProcessSetQuota = PROCESS_SET_QUOTA,
        ProcessSetInformation = PROCESS_SET_INFORMATION,
        ProcessQueryInformation = PROCESS_QUERY_INFORMATION,
        ProcessSuspendResume = PROCESS_SUSPEND_RESUME,
        ProcessQueryLimitedInformation = PROCESS_QUERY_LIMITED_INFORMATION,
        ProcessSetLimitedInformation = PROCESS_SET_LIMITED_INFORMATION,
        ProcessAllAccess = PROCESS_ALL_ACCESS
    };
    
}
