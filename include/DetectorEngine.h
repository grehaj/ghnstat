#pragma once

#include <TrafficValidator.h>
#include <memory>
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

    std::unique_ptr<validation::Validator> traffic_validator{validation::create_traffic_validaor()};
};
}
