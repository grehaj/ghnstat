#include "Alarm.h"

#include <algorithm>
#include <iostream>

namespace scan_detector
{

void Alarm::send(TrafficStatus status, const std::string& msg)
{
    if(traffic_alarm.find(status) == traffic_alarm.end())
    {
        std::cout << "*************************ALARM******************************\n" << msg << std::endl;
    }
    traffic_alarm[status] = msg;
}
}
