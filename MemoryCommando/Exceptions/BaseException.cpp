#include "BaseException.h"

#include <utility>

namespace  MemoryCommando::Exceptions {
    BaseException::BaseException(std::string errorMessage): _errorMessage(std::move(errorMessage)) {
    }

    std::string BaseException::GetErrorMessage() const {
        return _errorMessage;
    }
}
