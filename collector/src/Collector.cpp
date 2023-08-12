#include "Collector.h"
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

namespace collector
{
namespace fs = std::filesystem;

Collector::Collector(const std::string& ifc, file_count_t fc, storage_size_t s, const std::string& d):
    interface{ifc}, ip{get_interface_ip(ifc)}, file_count{fc}, storage_size{s}, directory{d},
    traffic_storage{interface, ip, storage_size}
{
}

void Collector::run()
{
    const std::string tcp_dump_command = std::string{"tcpdump -n -tt -i "} + interface + " dst " + ip;
    f = std::shared_ptr<FILE>(popen(tcp_dump_command.c_str(), "r"), utils::fifo_deleter<FILE>());
    if(f == nullptr)
        throw std::runtime_error{"SystemCommand: popen - tcpdump"};

    run_collector_threads();
}

void Collector::run_collector_threads()
{
    std::mutex storage_mutex;
    std::condition_variable ready_to_write;
    bool finished = false;
    ThreadArg threadArg{file_count, storage_size, directory};
    std::thread reader{TrafficReader{f, traffic_storage, storage_mutex, ready_to_write, finished}, threadArg};
    std::thread writter{TrafficWritter{traffic_storage, storage_mutex, ready_to_write, finished}, threadArg};
    reader.join();
    writter.join();
}

std::string Collector::get_interface_ip(const std::string& ifc) const
{
    auto ifcs{utils::get_active_interfaces_ip()};
    if(ifcs.find(ifc) == ifcs.end())
        throw std::runtime_error{"Unable to find interface '" + ifc + "'."};

    return ifcs[ifc];
}
}
