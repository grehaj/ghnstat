#include "TrafficStorage.h"

#include <iomanip>

namespace scan_detector
{

TrafficStorage::TrafficStorage(size_type s) : max_secs{s}
{
}

void TrafficStorage::update(time_t t, const TrafficData& data)
{
    ++per_port_trafic[t][data.dst.port];
    if(per_port_trafic.size() > max_secs)
    {
        per_port_trafic.erase(per_port_trafic.begin());
    }
}

bool TrafficStorage::is_full() const
{
    return per_port_trafic.size() == max_secs;
}

std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts)
{
    for(const auto& [t, d] : ts.per_port_trafic)
    {
        auto loct{std::localtime(&t)};
        out << "Time: " << std::put_time(loct, "%c") << std::endl;
        for(const auto& [port, count] : d)
        {
            out << "port: " << port << " accessed " << count << " times." << std::endl;
        }
        out << std::string(20, '-');
    }
    return out;
}

}
