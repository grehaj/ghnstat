#include "TrafficData.h"

#include <arpa/inet.h>

namespace scan_detector
{
uint32_t str_to_ip(const std::string& ip)
{
    in_addr add;

    inet_pton(AF_INET, ip.c_str(), &(add));

    return add.s_addr;
}

std::string ip_to_str(uint32_t ip)
{
    in_addr add{ip};
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(add), str, INET_ADDRSTRLEN);

    return std::string(str);
}

uint16_t str_to_port(const std::string& port)
{
    return std::stoi(port.c_str());
}
std::string port_to_str(uint16_t port)
{
    return std::to_string(port);
}
}
