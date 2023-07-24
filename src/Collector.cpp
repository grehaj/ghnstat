#include "Collector.h"
#include "Exceptions.h"
#include "TrafficReader.h"
#include "TrafficStorage.h"
#include "TrafficWritter.h"
#include "Utils.h"
#include <condition_variable>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <regex>
#include <thread>
#include <set>

namespace collector
{

namespace fs = std::filesystem;

Collector::Collector(const char* ifc, unsigned sec): max_sec{sec}, traffic_storage{max_sec}
{
    auto ifcs{utils::get_active_interfaces_ip()};
    if(ifcs.find(ifc) == ifcs.end())
        throw error::UsageError{"Unable to find interface '" + interface + "'."};

    interface = ifc;
    ip = ifcs[interface];
}

Collector::~Collector()
{
    if(f)
        pclose(f);
    f = nullptr;
}

void Collector::run()
{
    const std::string tcp_dump_command = std::string{"tcpdump -n -tt -i "} + interface + " dst " + ip;
    f = popen(tcp_dump_command.c_str(), "r");
    if(f == nullptr)
        throw error::SystemCommandError{"SystemCommand: popen - tcpdump"};

    prepare_filesystem();
    run_collector_threads();
}

void Collector::prepare_filesystem() const
{
    const fs::path p{utils::DEFAULT_LOG_LOCATION};
    if(fs::exists(p))
    {
        if(not fs::is_regular_file(p))
            error::UsageError{p.string() + " exists but it is not a regular file."};
    }
}

void Collector::run_collector_threads()
{
    std::mutex storage_mutex;
    std::condition_variable ready_to_write;
    std::thread reader{TrafficReader{f, traffic_storage, storage_mutex, ready_to_write}, max_sec};
    std::thread writter{TrafficWritter{traffic_storage, storage_mutex, ready_to_write}, max_sec};
    reader.join();
    writter.join();
}
}
