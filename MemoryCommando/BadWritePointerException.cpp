#include "BadWritePointerException.h"

MemoryCommando::Exceptions::BadWritePointerException::BadWritePointerException(const std::string& errorMessage,
                                                                               uintptr_t address):
    BaseException(errorMessage),
    Address(address) {
}
