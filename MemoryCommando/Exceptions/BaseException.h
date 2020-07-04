#pragma once
#include <exception>
#include <string>

namespace MemoryCommando::Exceptions {
    class BaseException : std::exception {
    protected:
        std::string ErrorMessage;
    public:
        BaseException(std::string errorMessage);
    };
}

