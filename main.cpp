#include "Collector.h"
#include "Exceptions.h"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace collector;

void usage(const char* app, const std::string& msg)
{
    std::cerr << "Error: " << msg << std::endl;
    std::cerr << "Usage: " << app << " interface1 sec" << std::endl;
}

int main(int argc, char* argv[]) try
{
    auto collector = collector::Collector{argv[1], stoi(argv[2])};
    collector.run();
    std::exit(EXIT_SUCCESS);
}
catch (const error::UsageError& e)
{
    usage(argv[0], e.what());
    std::exit(EXIT_FAILURE);

}
