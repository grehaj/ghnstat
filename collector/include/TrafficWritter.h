#pragma once

#include "CollectorThread.h"
#include "Exceptions.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <condition_variable>
#include <mutex>
#include <regex>
#include <string>

namespace collector
{
class TrafficWritter : public CollectorThread
{
public:
    TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv, bool& f);

    void operator()(ThreadArg threadArg) override;
};
}
