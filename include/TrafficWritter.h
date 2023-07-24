#pragma once

#include "Exceptions.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <condition_variable>
#include <mutex>
#include <regex>
#include <string>

namespace collector
{

const std::string stats_dir{"/tmp/tcpstat_logs"};

class TrafficWritter
{
public:
    TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv);

    void operator()(unsigned sec, utils::file_count_t nxt);

private:
    TrafficStorage& traffic_storage;
    std::mutex& storage_mtx;
    std::condition_variable& ready_to_write;
};
}
