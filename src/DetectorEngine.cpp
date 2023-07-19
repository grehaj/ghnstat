#include "DetectorEngine.h"
#include "Exceptions.h"
#include "TrafficData.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <regex>

#include <iostream>

namespace scan_detector
{
void DetectorEngine::run()
{
    const auto ips{utils::get_active_interfaces_ip()};
//     TODO handle no if and more ifaces
//    if(ips.size() != 1)
//        throw error::DetectorError{"Single active network interface required."};

    utils::SystemCommand cmd{std::string{"tcpdump -n dst "} + ips[0]};
    std::optional<std::string> s;
    TrafficStorage traffic_storage;

    std::regex r{R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
    std::smatch sm;
    // Alarm thread and notify when get anomaly
    while ((s = cmd.get_next_output_line()).has_value())
    {
        if(std::regex_search(*s, sm, r))
        {
            const auto d = TrafficData{{str_to_ip(sm[1]), str_to_port(sm[2])}, {str_to_ip(sm[3]), str_to_port(sm[4])}};
            traffic_storage.update(d);
            std::cout << sm[1] << std::endl;
        }
    }
}

}
