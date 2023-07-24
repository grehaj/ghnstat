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
    run_collector_threads(get_next_file_number());
}

void Collector::prepare_filesystem() const
{
    const fs::path p{stats_dir};
    if(not fs::exists(p))
        fs::create_directory(p);
    else
    {
        if(not fs::is_directory(p))
            error::UsageError{stats_dir + " already exists and is not a directory"};
    }
}

file_count_t Collector::get_next_file_number() const
{
    const fs::path p{stats_dir};
    std::set<file_count_t> file_numbers;
    std::regex r{R"(stats(\d+))"};
    std::smatch sm;
    for(const fs::directory_entry& e : fs::directory_iterator{p})
    {
        if(fs::is_regular_file(e))
        {
            const std::string file_name = e.path().filename().string();
            if (std::regex_match(file_name, sm, r))
            {
                file_numbers.insert(std::stoull(sm[1]));
            }
        }
    }

    if(file_numbers.empty())
        return 1;

    const auto num = *file_numbers.rbegin();
    if(num == 0 or num >= 3)
        return 1;

    return num + 1;
}

void Collector::run_collector_threads(file_count_t next_file_number)
{
    std::mutex storage_mutex;
    std::condition_variable ready_to_write;
    std::thread reader{TrafficReader{f, traffic_storage, storage_mutex, ready_to_write}, max_sec};
    std::thread writter{TrafficWritter{traffic_storage, storage_mutex, ready_to_write}, max_sec, next_file_number};
    reader.join();
    writter.join();
}
}
