#pragma once

#include "TrafficStorage.h"
#include "Utils.h"
#include <string>

namespace collector
{
class Collector
{
public:
    Collector(const char* ifc, unsigned mc);
    ~Collector();
    void run();

private:
    void prepare_filesystem() const;
    void run_collector_threads();

    std::string interface;
    std::string ip;
    const unsigned max_sec;
    FILE* f{nullptr};
    TrafficStorage traffic_storage;
};
}
