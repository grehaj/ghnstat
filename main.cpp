#include "Collector.h"

int main(int argc, char* argv[])
{
    auto detector = scan_detector::Collector{};
    detector.run();
    return 0;
}
