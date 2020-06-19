#pragma once

namespace MemoryCommanderCPP {
    enum class MemoryProtection {

        /// Disables all access to the region of virtual memory
        ZeroAccess = 0x00,

        /// Disables all access to the region of virtual memory
        NoAccess = 0x01,

        /// Marks the region of virtual memory as readable
        ReadOnly = 0x02,

        /// Marks the region of virtual memory as readable and/or writable
        ReadWrite = 0x04,

        /// Marks the region of virtual memory as readable and/or copy on write
        WriteCopy = 0x08,

        /// Marks the region of virtual memory as executable
        Execute = 0x10,

        /// Marks the region of virtual memory as readable and/or executable
        ExecuteRead = 0x20,

        /// Marks the region of virtual memory as readable, writable and/or executable
        ExecuteReadWrite = 0x40,

        /// Marks the region of virtual memory as readable, copy on write and/or executable
        ExecuteWriteCopy = 0x80,

        /// Marks the region of virtual memory as guarded
        Guard = 0x100,

        /// Marks the region of virtual memory as readable, writable and guarded
        ReadWriteGuard = 0x104,

        /// Marks the region of virtual memory as non-cacheable 
        NoCache = 0x200
    };
}
