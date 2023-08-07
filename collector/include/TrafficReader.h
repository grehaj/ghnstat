#pragma once

#include "Exceptions.h"
#include "CollectorThread.h"
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

class TrafficReader : public CollectorThread
{
public:
    TrafficReader(std::shared_ptr<FILE> data_src, TrafficStorage& ts, std::mutex& m, std::condition_variable& cv, bool& f);

   void operator()(ThreadArg threadArg) override;

private:
    std::shared_ptr<FILE> data_source;

    inline static const std::regex r{
        R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
};

}
