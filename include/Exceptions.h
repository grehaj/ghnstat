#pragma once

#include <stdexcept>
#include <string>

namespace scan_detector
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

class DetectorError : public std::runtime_error
{
public:
    DetectorError(const std::string& msg) : std::runtime_error{msg}
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

}
}

