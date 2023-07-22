#include "DetectorEngine.h"
#include "Exceptions.h"
#include "TrafficData.h"
#include "TrafficStorage.h"
#include "TrafficValidator.h"
#include "Utils.h"
#include <regex>
#include <ctime>

#include <iostream>

namespace scan_detector
{

using namespace utils;

DetectorEngine::DetectorEngine(const char* ifc, int sec): interface{}, seconds{sec}
{
    auto ifcs{utils::get_active_interfaces_ip()};
    if(ifcs.find(ifc) == ifcs.end())
        throw error::UsageError{"Unable to find interface '" + interface + "'."};

    interface = ifcs[std::string{ifc}];
}

void DetectorEngine::run()
{
    const std::string tcp_dump_command = std::string{"tcpdump -n -tt dst "} + interface;
    FILE* f = popen(tcp_dump_command.c_str(), "r");
    if(f == nullptr)
        throw error::SystemCommandError{"SystemCommand: popen - tcpdump"};

    TrafficStorage traffic_storage(seconds);
    std::regex r{R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
    std::smatch sm;
    char buffer[utils::RSIZE]{};
    while (fgets(buffer, utils::RSIZE, f))
    {
        std::string s{buffer};
        if(std::regex_search(s, sm, r))
        {
            auto d = ProtocolData{{str_to_ip(sm[1]), str_to_port(sm[2])}, {str_to_ip(sm[3]), str_to_port(sm[4])}};
            auto end_pos = s.find(".");
            time_t tmime_stamp = std::stoull(s.substr(0, end_pos));
            const auto& latest_data = traffic_storage.update(tmime_stamp, d);
            validate(latest_data);
        }
    }
    pclose(f);
}

}
