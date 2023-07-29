#pragma once

#include <stdexcept>
#include <string>

namespace collector
{
namespace error
{
class CollectorError : public std::runtime_error
{
public:
    CollectorError(const std::string& msg) : std::runtime_error{msg}
    {
    }
};

class SystemCommandError : public std::runtime_error
{
public:
    SystemCommandError(const std::string& msg) : std::runtime_error{msg}
    {
    }
};

class UsageError : public std::invalid_argument
{
public:
    UsageError(const std::string& msg) : std::invalid_argument{msg}
    {
    }
};

}
}

