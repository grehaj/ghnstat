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
    utils::file_count_t get_next_file_number() const;
    void run_collector_threads(utils::file_count_t next_file_number);

    std::string interface;
    std::string ip;
    const unsigned max_sec;
    FILE* f{nullptr};
    TrafficStorage traffic_storage;
};
}
