#include "Utils.h"
#include "Exceptions.h"
#include <algorithm>
#include <arpa/inet.h>
#include <cstdio>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

namespace collector
{
namespace utils
{

std::map<std::string, std::string> get_active_interfaces_ip()
{
    FILE* f = popen("ip -4 addr", "r");
    if(f == nullptr)
        throw error::SystemCommandError{"SystemCommand: popen - ip addr"};

    std::regex r{R"(inet\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))"};
    std::smatch smtch;
    std::map<std::string, std::string> active_interfaces_ip;
    char buffer[RSIZE]{};

    while (fgets(buffer, RSIZE, f))
    {
        std::string s{buffer};
        if(std::regex_search(s, smtch, r))
        {
            auto start = s.find_last_of(" ") + 1;
            auto ifc_name = s.substr(start);
            if(ifc_name.back() == '\n')
                ifc_name.erase(ifc_name.size() - 1);
            active_interfaces_ip[ifc_name] = smtch[1];
        }
    }

    pclose(f);
    return active_interfaces_ip;
}

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
}
