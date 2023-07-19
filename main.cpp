#include "DetectorEngine.h"

int main(int argc, char* argv[])
{
    // TODO run app not engine
    auto detector = scan_detector::DetectorEngine{};
    detector.run();
    return 0;
}
