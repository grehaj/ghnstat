#pragma once

#include <cstdint>
#include <istream>
#include <string>

namespace scan_detector
{
struct SocketData
{
    uint32_t ip;
    uint16_t port;
};

struct TrafficData
{
    SocketData src;
    SocketData dst;
};

uint32_t str_to_ip(const std::string&);
std::string ip_to_str(uint32_t);
uint16_t str_to_port(const std::string&);
std::string port_to_str(uint16_t);
}
