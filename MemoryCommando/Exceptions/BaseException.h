#pragma once
#include <exception>
#include <string>

namespace MemoryCommando::Exceptions {
    class BaseException : std::exception {
    protected:
        std::string _errorMessage;
    public:
        BaseException(std::string errorMessage);
        std::string GetErrorMessage() const;
    };
}

