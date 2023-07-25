#include "TrafficWritter.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include <iostream>

namespace collector
{

namespace fs = std::filesystem;

std::string get_file_name(int num)
{
    char buffer[100]{};
    sprintf(buffer, utils::LOG_LOCATION.c_str(), num);

    return std::string{buffer};
}

TrafficWritter::TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv):
        traffic_storage{ts}, storage_mtx{m}, ready_to_write{cv}
{
}

void TrafficWritter::operator()(utils::file_count_t max_count, bool& finished)
{
    file_count_t file_num = 1;
    fs::path file_path = fs::path{get_file_name(file_num)};

    while(true)
    {
        std::unique_lock<std::mutex> ul{storage_mtx};
        ready_to_write.wait(ul, [&]{return traffic_storage.size() == utils::MAX_STORAGE_SIZE;});
        std::ofstream out{file_path.string()};
        out << traffic_storage;
        traffic_storage.clear();
        ++file_num;
        std::cout << file_num << " " << max_count << std::endl;
        if(file_num > max_count)
        {
            finished = true;
            return;
        }
        file_path = fs::path{get_file_name(file_num)};
    }
}
}
