#include "TrafficValidator.h"

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

namespace scan_detector
{

using namespace std::chrono_literals;

static void alarm(std::string msg)
{
    std::this_thread::sleep_for(10s);
    std::cout << "*************************ALARM******************************\n" << msg << std::endl;
    //TODO write data via a socket to the server
}

void validate(const PortTraffic& td)
{
    std::ostringstream buffer;
    buffer << td;
    if(td.total_count > 100)
    {
        alarm(buffer.str());
    }
}
}
