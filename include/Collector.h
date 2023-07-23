#pragma once

#include <string>

namespace collector
{
class Collector
{
public:
    Collector(const char* ifc, int sec);
    ~Collector();
    void run();

private:
    std::string interface;
    int seconds;
    FILE* f{nullptr};
};
}
