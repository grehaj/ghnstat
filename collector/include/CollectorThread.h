#pragma once

#include "TrafficStorage.h"
#include "Utils.h"
#include <condition_variable>
#include <mutex>

namespace collector
{
struct ThreadArg
{
    utils::file_count_t file_count;
    utils::storage_size_t storage_size;
    std::string directory;
};

class CollectorThread
{
public:
    CollectorThread(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv, bool& f);
    virtual ~CollectorThread() = default;
    virtual void operator()(ThreadArg threadArg) = 0;

protected:
    TrafficStorage& traffic_storage;
    std::mutex& storage_mtx;
    std::condition_variable& ready_to_write;
    bool& finished;
};
}
