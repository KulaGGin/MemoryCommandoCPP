#pragma once
#include <exception>
#include <string>

namespace MemoryCommando::Exceptions {
    class BaseException : public std::exception {
    public:
        BaseException(std::string errorMessage);

        std::string GetErrorMessage() const;
    protected:
        std::string _errorMessage;
    };
}