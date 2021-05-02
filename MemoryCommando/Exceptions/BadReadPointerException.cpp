#include "BadReadPointerException.h"

namespace MemoryCommando::Exceptions {
    BadReadPointerException::BadReadPointerException(const std::string& errorMessage, uintptr_t address):
        BaseException(errorMessage),
        Address(address) {
    }
}
