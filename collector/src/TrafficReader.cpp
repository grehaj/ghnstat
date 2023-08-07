#include "TrafficReader.h"

namespace collector
{
using namespace utils;

TrafficReader::TrafficReader(std::shared_ptr<FILE> data_src, TrafficStorage& ts, std::mutex& m,
                             std::condition_variable& cv, bool& f):
    CollectorThread{ts, m, cv, f}, data_source{data_src}
{
}

void TrafficReader::operator()(ThreadArg threadArg)
{
    std::smatch sm;
    char buffer[utils::READSIZE]{};
    while (not finished and fgets(buffer, utils::READSIZE, data_source.get()))
    {
        std::string s{buffer};
        if(std::regex_search(s, sm, r))
        {
            auto d = Connection{{str_to_ip(sm[1]), str_to_port(sm[2])}, {str_to_ip(sm[3]), str_to_port(sm[4])}};
            auto end_pos = s.find(".");
            time_t tmime_stamp = std::stoull(s.substr(0, end_pos));
            {
                std::lock_guard<std::mutex> lg{storage_mtx};
                traffic_storage.update(tmime_stamp, d);
            }
            if(traffic_storage.size() == threadArg.storage_size)
                ready_to_write.notify_one();
        }
    }
   }
}
