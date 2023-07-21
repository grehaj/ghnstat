#pragma once

#include "TrafficData.h"
#include <cstdint>
#include <deque>

namespace scan_detector
{

class TrafficStorage
{
private:
    using traffic_t = std::deque<PortTraffic>;
    using size_type = typename traffic_t::size_type;

    const size_type max_secs;
    traffic_t traffic;

public:
    explicit TrafficStorage(size_type s);
    const PortTraffic& update(time_t t, const ProtocolData& data);

    friend std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts);
};

}
