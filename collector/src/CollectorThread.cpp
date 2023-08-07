#include "CollectorThread.h"

namespace collector
{
CollectorThread::CollectorThread(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv, bool& f):
    traffic_storage{ts}, storage_mtx{m}, ready_to_write{cv}, finished{f}
{

}
}
