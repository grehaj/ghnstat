#include "TrafficStorage.h"

namespace scan_detector
{

void TrafficStorage::update(const TrafficData& data)
{
    ++per_port_trafic[data.dst.port];
}

}
