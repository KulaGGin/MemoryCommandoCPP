#include "BaseException.h"

#include <utility>

namespace  MemoryCommando::Exceptions {
    BaseException::BaseException(std::string errorMessage) : ErrorMessage(std::move(errorMessage)) {
    }
}
