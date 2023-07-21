#include "Exceptions.h"
#include "ScanDetector.h"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace scan_detector;

void usage(const char* app, const std::string& msg)
{
    std::cerr << "Error: " << msg << std::endl;
    std::cerr << "Usage: " << app << " interface1 sec" << std::endl;
}



int main(int argc, char* argv[]) try
{
    scan_detector::detect_scan(argv[1], stoi(argv[2]));
    std::exit(EXIT_SUCCESS);
}
catch (const error::UsageError& e)
{
    usage(argv[0], e.what());
    std::exit(EXIT_FAILURE);

}
