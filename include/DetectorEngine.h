#pragma once

#include <mutex>
#include <string>

namespace scan_detector
{
class DetectorEngine
{
public:
    DetectorEngine(const char* ifc, int sec);
    void run();

private:
    std::string interface;
    int seconds;
};
}
