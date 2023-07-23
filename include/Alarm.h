#pragma once

#include "TrafficData.h"
#include <map>
#include <string>

namespace scan_detector
{
class Alarm
{
private:
    std::map<TrafficStatus, std::string> traffic_alarm;
public:
    void send(TrafficStatus status, const std::string& msg);
};
}
