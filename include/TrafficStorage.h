#pragma once

#include "TrafficData.h"
#include <cstdint>
#include <deque>

namespace collector
{

class TrafficStorage
{
private:
    using size_type = typename traffic_t::size_type;

    const size_type max_secs;
    traffic_t traffic;

public:
    explicit TrafficStorage(size_type s);

    bool update(time_t t, const Connection& data);
    size_type size() const
    {
        return traffic.size();
    }
    void clear()
    {
        return traffic.clear();
    }

    friend std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts);
};

}
