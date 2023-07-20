#include "ScanDetector.h"
#include "DetectorEngine.h"

namespace scan_detector
{
void detect_scan(const char* interface, int sec)
{
    auto detector = scan_detector::DetectorEngine{interface, sec};
    detector.run();
}
}
