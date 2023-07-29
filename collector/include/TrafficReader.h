#pragma once

#include "Exceptions.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <condition_variable>
#include <fstream>
#include <memory>
#include <mutex>
#include <regex>

namespace collector
{
using namespace utils;

class TrafficReader
{
public:
    TrafficReader(std::shared_ptr<FILE> data_src, TrafficStorage& ts, std::mutex& m, std::condition_variable& cv);

   void operator()(utils::storage_size_t s, bool& finished);

private:
    std::shared_ptr<FILE> data_source;
    TrafficStorage& traffic_storage;
    std::mutex& storage_mtx;
    std::condition_variable& ready_to_write;

    inline static const std::regex r{
        R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
};

}
