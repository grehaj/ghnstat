#include "DetectorEngine.h"
#include "Exceptions.h"
#include "TrafficData.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <regex>
#include <ctime>

#include <iostream>

namespace scan_detector
{

DetectorEngine::DetectorEngine(const char* ifc, int sec): interface{}, seconds{sec}
{
    auto ifcs{utils::get_active_interfaces_ip()};
    if(ifcs.find(ifc) == ifcs.end())
        throw error::UsageError{"Unable to find interface '" + interface + "'."};

    interface = ifcs[std::string{ifc}];
}

void DetectorEngine::run()
{
    utils::SystemCommand cmd{std::string{"tcpdump -n -tt dst "} + interface};
    std::optional<std::string> s;
    TrafficStorage traffic_storage(seconds);

    std::regex r{R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
    std::smatch sm;
    // Alarm thread and notify when anomaly detected
    while ((s = cmd.get_next_output_line()).has_value())
    {
        if(std::regex_search(*s, sm, r))
        {
            auto d = TrafficData{{str_to_ip(sm[1]), str_to_port(sm[2])}, {str_to_ip(sm[3]), str_to_port(sm[4])}};
            auto end_pos = s->find(".");
            time_t tmime_stamp = std::stoull(s->substr(0, end_pos));
            traffic_storage.update(tmime_stamp, d);
            // TODO remove this
            if(traffic_storage.is_full())
                std::cout << traffic_storage << std::endl;
        }
    }
}

}
