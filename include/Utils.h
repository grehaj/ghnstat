#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace collector
{
namespace utils
{
using port_t = uint16_t;
using ip_t = uint32_t;
using counter_t = uint64_t;
using file_count_t = uint64_t;

constexpr int RSIZE{1024};
constexpr uint64_t DEFAULT_FILE_MAX_SIZE{1000000};
const std::string DEFAULT_LOG_LOCATION{"/tmp/tcpstat_logs"};

ip_t str_to_ip(const std::string&);
std::string ip_to_str(ip_t);
port_t str_to_port(const std::string&);
std::string port_to_str(port_t);
std::map<std::string, std::string> get_active_interfaces_ip();
}
}
