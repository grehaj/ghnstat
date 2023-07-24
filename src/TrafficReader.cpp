#include "TrafficReader.h"

namespace collector
{
using namespace utils;

TrafficReader::TrafficReader(FILE* data_src, TrafficStorage& ts, std::mutex& m, std::condition_variable& cv):
        data_source{data_src}, traffic_storage{ts}, storage_mtx{m}, ready_to_write{cv}
{
}

void TrafficReader::operator()(int sec)
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
}
