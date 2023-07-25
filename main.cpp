#include "Collector.h"
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;
using namespace collector;

void usage(const char* app)
{
    std::cerr << "Usage: " << app << " interface max_count\n";
    std::cerr << "Collects network traffic received on 'interface' and creates summary in json format"
                 " to " << utils::LOG_LOCATION << "\n";
    std::cerr << "\tinterface: lo|wlan0|eth0|...\n";
    std::cerr << "\tmax_count: positive number indicating numer of files generated in format "
              << utils::LOG_LOCATION << ". Each file covers 10 seconds period.\n";
}

int main(int argc, char* argv[]) try
{
    for(int i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "-h" or std::string(argv[i]) == "help" or std::string(argv[i]) == "--help")
        {
            usage(argv[0]);
            std::exit(EXIT_SUCCESS);
        }
    }

    if(argc != 3)
    {
        usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }

    int num = stoi(argv[2]);
    if(num <= 0)
    {
        usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }

    const uint8_t file_num = static_cast<uint8_t>(num);
    collector::Collector{argv[1], file_num}.run();
    std::exit(EXIT_SUCCESS);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    usage(argv[0]);
    std::exit(EXIT_FAILURE);
}
