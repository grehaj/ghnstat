#pragma once

#include <TrafficData.h>
#include <ctime>
#include <cstdint>
#include <map>

namespace scan_detector
{

class TrafficStorage
{
private:
    using traffic_t = std::map<time_t, std::map<uint16_t, uint64_t>>;
    using size_type = typename traffic_t::size_type;

    const size_type max_secs;
    traffic_t per_port_trafic;

public:
    explicit TrafficStorage(size_type s);

    void update(time_t, const TrafficData& data);
    bool is_full() const;

    friend std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts);
};

}
