#include "Exceptions.h"
#include "ScanDetector.h"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace scan_detector;

void usage(const char* app, const std::string& msg)
{
//    multiple detection threads support
//    std::cerr << "Usage: " << app << " interface1 sec11 sec12 ... [interface2 sec21 sec22 ... ]" << std::endl;
    std::cerr << "Error: " << msg << std::endl;
    std::cerr << "Usage: " << app << " interface1 sec11 sec12 ..." << std::endl;
}



int main(int argc, char* argv[]) try
{
//    TODO Parsing and error handling
//    scan_detector::detect_scan(argv[1], argv[2]);
    scan_detector::detect_scan("wlp2s0", 60);

    std::exit(EXIT_SUCCESS);
}
catch (const error::UsageError& e)
{
    usage(argv[0], e.what());
    std::exit(EXIT_FAILURE);

}
