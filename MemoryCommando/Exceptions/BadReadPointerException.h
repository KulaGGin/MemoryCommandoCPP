#pragma once
#include "Exceptions/BaseException.h"

namespace MemoryCommando::Exceptions {
    class BadReadPointerException : public BaseException {
    public:
        BadReadPointerException(const std::string& errorMessage, uintptr_t address);

        uintptr_t Address;
    };
}

