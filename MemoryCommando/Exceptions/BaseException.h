#pragma once
#include <exception>
#include <string>

namespace MemoryCommando::Exceptions {
    class BaseException : public std::exception {
    protected:
        std::string _errorMessage;
    public:
        BaseException(std::string errorMessage);
        std::string GetErrorMessage() const;
    };
}