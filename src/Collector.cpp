#include "Collector.h"
#include "CollectorUtils.h"
#include "Exceptions.h"
#include <algorithm>
#include <cstdlib>
#include <regex>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <queue>

//TODO delete iostream  when not needed
#include <iostream>

namespace scan_detector
{
void Collector::run()
{
    const auto& ips{utils::get_active_interfaces_ip()};
    if(ips.empty())
        throw error::CollectorError{"Collector: No active network interfaces detected."};

    utils::SystemCommand cmd{"tcpdump -n"};
    std::optional<std::string> s;

    while ((s = cmd.get_next_output_line()).has_value())
    {
        std::cout << *s;
    }
}
}
