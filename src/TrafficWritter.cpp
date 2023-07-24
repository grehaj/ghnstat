#include "TrafficWritter.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace collector
{

namespace fs = std::filesystem;

TrafficWritter::TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv):
        traffic_storage{ts}, storage_mtx{m}, ready_to_write{cv}
{
}

void TrafficWritter::operator()(unsigned sec, utils::file_count_t nxt)
{
    fs::path file_path = fs::path{stats_dir} / ("stats" + std::to_string(nxt));

    {
        std::ofstream out{file_path.string()};
        out << "**********************************************************\n";
    }
    while(true)
    {
        std::unique_lock<std::mutex> ul{storage_mtx};
        ready_to_write.wait(ul, [&]{return traffic_storage.size() == sec;});
        std::ofstream out{file_path.string(), std::ios::app};
        out << traffic_storage;
        traffic_storage.clear();
        if(fs::file_size(file_path) > 1000000)
        {
            out.close();
            ++nxt;
            if(nxt > 3)
                nxt = 1;
            file_path = fs::path{stats_dir} / ("stats" + std::to_string(nxt));
            std::ofstream out{file_path.string()};
            out << "**********************************************************\n";
        }
    }
}
}
