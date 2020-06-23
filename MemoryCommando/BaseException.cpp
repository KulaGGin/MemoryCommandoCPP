#include "BaseException.h"

#include <utility>

namespace  MemoryCommanderCpp::Exceptions {
    BaseException::BaseException(std::string errorMessage) : ErrorMessage(std::move(errorMessage)) {
    }
}
