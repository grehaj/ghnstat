#pragma once

#include <optional>
#include <map>
#include <string>

namespace collector
{
namespace utils
{

constexpr int RSIZE{1024};

std::map<std::string, std::string> get_active_interfaces_ip();

uint32_t str_to_ip(const std::string&);
std::string ip_to_str(uint32_t);
uint16_t str_to_port(const std::string&);
std::string port_to_str(uint16_t);

using file_count_t = unsigned long long;

}
}
