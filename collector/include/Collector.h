#pragma once

#include "TrafficStorage.h"
#include "Utils.h"
#include <memory>
#include <string>

namespace collector
{
class Collector
{
public:
    Collector(const std::string& ifc, file_count_t fc, storage_size_t s, const std::string& d="");
    void run();

private:
    std::string get_interface_ip(const std::string& ifc) const;

    const std::string interface;
    const std::string ip;
    const file_count_t file_count;
    const storage_size_t storage_size;
    const std::string directory;

    TrafficStorage traffic_storage;
};
}
