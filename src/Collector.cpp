#include "Collector.h"
#include "Exceptions.h"
#include "TrafficData.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <fstream>
#include <regex>
#include <ctime>

#include <condition_variable>
#include <mutex>
#include <thread>


namespace collector
{

using namespace utils;

class TrafficReader
{
public:
    TrafficReader(FILE* data_src, TrafficStorage& ts, std::mutex& m, std::condition_variable& cv):
        data_source{data_src}, traffic_storage{ts}, storage_mtx{m}, ready_to_write{cv}
    {
    }


   void operator()(int sec, std::string interface)
   {
       std::smatch sm;
       char buffer[utils::RSIZE]{};
       while (fgets(buffer, utils::RSIZE, data_source))
       {
           std::string s{buffer};
           if(std::regex_search(s, sm, r))
           {
               auto d = ProtocolData{{str_to_ip(sm[1]), str_to_port(sm[2])}, {str_to_ip(sm[3]), str_to_port(sm[4])}};
               auto end_pos = s.find(".");
               time_t tmime_stamp = std::stoull(s.substr(0, end_pos));
               {
                   std::lock_guard<std::mutex> lg{storage_mtx};
                   traffic_storage.update(tmime_stamp, d);
               }
               if(traffic_storage.size() == sec)
                   ready_to_write.notify_one();
           }
       }
   }

private:
    FILE* data_source{nullptr};
    TrafficStorage& traffic_storage;
    std::mutex& storage_mtx;
    std::condition_variable& ready_to_write;

    inline static const std::regex r{
        R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
};

class TrafficWritter
{
public:
    TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv):
        traffic_storage{ts}, storage_mtx{m}, ready_to_write{cv}
    {
        std::ofstream out{"/tmp/stats.log"};
        out << "**********************************************************\n";
    }


   void operator()(int sec)
   {
       while(true)
       {
           std::unique_lock<std::mutex> ul{storage_mtx};
           ready_to_write.wait(ul, [&]{return traffic_storage.size() == sec;});
           std::ofstream out{"/tmp/stats.log", std::ios::app};
           out << traffic_storage;
           traffic_storage.clear();
       }
   }

private:
    TrafficStorage& traffic_storage;
    std::mutex& storage_mtx;
    std::condition_variable& ready_to_write;

    std::ofstream out{"/tmp/stats.log"};
};

Collector::Collector(const char* ifc, int sec): interface{ifc}, seconds{sec}
{
    auto ifcs{utils::get_active_interfaces_ip()};
    if(ifcs.find(ifc) == ifcs.end())
        throw error::UsageError{"Unable to find interface '" + interface + "'."};
}

Collector::~Collector()
{
    if(f)
        pclose(f);
}

void Collector::run()
{
    const std::string tcp_dump_command = std::string{"tcpdump -n -tt -i "} + interface;
    f = popen(tcp_dump_command.c_str(), "r");
    if(f == nullptr)
        throw error::SystemCommandError{"SystemCommand: popen - tcpdump"};

    TrafficStorage traffic_storage(seconds);
    std::mutex storage_mutex;
    std::condition_variable ready_to_write;
    std::thread reader{TrafficReader{f, traffic_storage, storage_mutex, ready_to_write}, seconds, interface};
    std::thread writter{TrafficWritter{traffic_storage, storage_mutex, ready_to_write}, seconds};
    reader.join();
    writter.join();
}

}
