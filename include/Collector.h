#pragma once

#include "TrafficStorage.h"
#include "Utils.h"
#include <string>

namespace collector
{
class Collector
{
public:
    Collector(const char* ifc, file_count_t fc);
    ~Collector();
    void run();

private:
    void prepare_filesystem() const;
    void run_collector_threads();

    std::string interface;
    std::string ip;
    const file_count_t max_files;
    FILE* f{nullptr};
    TrafficStorage traffic_storage;
};
}
