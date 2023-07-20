#pragma once

#include <string>

namespace scan_detector
{
class DetectorEngine
{
public:
    DetectorEngine(const char* ifc, int sec);
    void run();

private:
    const std::string interface;
    int seconds;
};
}
