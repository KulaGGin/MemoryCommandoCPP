#pragma once
#include "Exceptions/BaseException.h"

namespace MemoryCommando::Exceptions {
    class BadWritePointerException : public BaseException {
    public:
        BadWritePointerException(const std::string& errorMessage, uintptr_t address);

        uintptr_t Address;
    };
}

