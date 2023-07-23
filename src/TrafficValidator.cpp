#include "TrafficValidator.h"

#include <chrono>
#include <future>

#include <set>
#include <string>
#include <sstream>

namespace scan_detector
{

namespace validation
{
void Validator::add_validation(std::unique_ptr<Validator>)
{
}

TrafficStatus ScanValidator::validate(const PortTraffic& pt) const
{
    if(pt.total_count >= max_port_threshold)
    {
        std::set<ip_t> src_hosts;

        for(const auto& [_, src_socket_data] : pt.amount_per_dest_port)
        {
            for(const auto& [source_socket, _] : src_socket_data.per_socket_count)
            {
                src_hosts.insert(source_socket.ip);
            }
        }

        if(src_hosts.size() >= distributed_port_scan_threshold)
            return TrafficStatus::nok_distributed_port_scan;
        else
            return TrafficStatus::nok_single_src_port_scan;
    }
    return TrafficStatus::ok;
}

void TrafficValidator::add_validation(std::unique_ptr<Validator> v)
{
    validators.push_back(std::move(v));
}

TrafficStatus TrafficValidator::validate(const PortTraffic& td) const
{
    for(const auto& validator : validators)
    {
        const auto status = validator->validate(td);
        if(status != TrafficStatus::ok)
        {
            std::ostringstream buffer;
            buffer << td;
            alarm.send(status, buffer.str());
        }
    }
    return TrafficStatus::ok;
}

std::unique_ptr<Validator> create_traffic_validaor()
{
    std::unique_ptr<Validator> validator = std::make_unique<TrafficValidator>();

    validator->add_validation(std::make_unique<ScanValidator>());
    // Add other validations here

    return validator;
}
}
}
