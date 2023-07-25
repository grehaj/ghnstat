#include "TrafficStorage.h"
#include "JsonConversion.h"
#include <iomanip>

namespace collector
{

TrafficStorage::TrafficStorage(size_type s) : max_secs{s}
{
}

bool TrafficStorage::update(time_t observation_time, const Connection& data)
{
    bool is_new_second{false};
    if(traffic.empty())
    {
        traffic.push_back(PortTraffic{observation_time});
    }

    const auto old_back = traffic.back();
    if(old_back.observation_time < observation_time)
    {
        for(auto t = old_back.observation_time + 1; t <= observation_time; ++t)
        {
            traffic.push_back(PortTraffic{t});
            is_new_second = true;
        }
    }

    auto& new_back = traffic.back();
    new_back += data;

    while(traffic.size() > max_secs)
    {
        traffic.pop_front();
    }

    return is_new_second;
}

std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts)
{
    if(ts.traffic.empty())
        return out;

    nlohmann::json j{};
    j["stats"] = ts.traffic;
    return out << std::setw(2) << j << std::endl;
}

}
