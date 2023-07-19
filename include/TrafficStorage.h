#pragma once

#include <TrafficData.h>
#include <ctime>
#include <cstdint>
#include <map>
#include <vector>

namespace scan_detector
{

class TrafficStorage
{
private:
    std::map<uint16_t, uint64_t> per_port_trafic;

public:
    void update(const TrafficData& data);
};
}
