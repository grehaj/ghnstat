#include "Collector.h"
#include <iostream>
#include <cstdlib>
#include <optional>
#include <string>
#include <unistd.h>

using namespace std;
using namespace collector;

void usage(const char* app)
{
    std::cerr << "Usage: " << app << " -i interface -c count -t secs\n";
    std::cerr << "Collects network traffic received on 'interface' and creates summary in json format\n";
    std::cerr << "  interface: lo|wlan0|eth0|...\n";
    std::cerr << "  count: number of files to be created - range 1 to " << utils::MAX_FILE_COUNT << "\n";
    std::cerr << "  secs: number of seconds covered by each file - range 1 to " << utils::MAX_STORAGE_SIZE << "\n";
    std::cerr << "  location: directory where logs should be stored\n";
    std::cerr << "Example:\n";
    std::cerr << app << " -i wlp2s0 -c 5 -t 10\n";
    std::cerr << "Collects statisticts from wlp2s0. "
                 "Generates 5 files each containing 10 seconds period and stores them to /tmp directory.\n";
}

int main(int argc, char* argv[]) try
{
    for(int i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "help" or std::string(argv[i]) == "--help")
        {
            usage(argv[0]);
            std::exit(EXIT_SUCCESS);
        }
    }
    int opt{-1};
    std::optional<std::string> ifc;
    std::optional<int> count;
    std::optional<int> secs;

    while ((opt = getopt(argc, argv, ":i:c:t:h")) != -1)
    {
        switch(opt)
        {
        case 'i':
            ifc = std::string{optarg};
            break;
        case 'c':
            count = std::stoi(optarg);
            break;
        case 't':
            secs = std::stoi(optarg);
            break;
        case 'h':
            usage(argv[0]);
            std::exit(EXIT_SUCCESS);
        }
    }

    if(not (ifc and secs and count))
    {
        usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }

    if(*secs <= 0 or *count <= 0 or *secs > 60 or *count > 60)
    {
        usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }

    const uint8_t file_count = static_cast<uint8_t>(*count);
    const uint8_t storage_size = static_cast<uint8_t>(*secs);
    collector::Collector{*ifc, file_count, storage_size}.run();
    std::exit(EXIT_SUCCESS);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
}
