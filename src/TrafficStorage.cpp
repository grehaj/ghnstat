#include "TrafficStorage.h"

namespace scan_detector
{

TrafficStorage::TrafficStorage(size_type s) : max_secs{s}
{
}

const PortTraffic& TrafficStorage::update(time_t observation_time, const ProtocolData& data)
{
    if(traffic.empty())
    {
        traffic.push_back(PortTraffic{observation_time});
    }

    const auto& old_back = traffic.back();
    if(old_back.observation_time < observation_time)
    {
        for(auto t = old_back.observation_time + 1; t <= observation_time; ++t)
            traffic.push_back(PortTraffic{t});
    }

    auto& new_back = traffic.back();
    new_back += data;

    while(traffic.size() > max_secs)
    {
        traffic.pop_front();
    }

    return traffic.back();
}

std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts)
{
    for(const auto& d : ts.traffic)
    {
        out << d << std::endl;
    }
    out << "******************************************************\n";
    return out;
}

}
