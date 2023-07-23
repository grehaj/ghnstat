#pragma once

#include <ctime>
#include <cstdint>
#include <map>
#include <string>
#include <ostream>

namespace collector
{

using port_t = uint16_t;
using ip_t = uint32_t;
using counter_t = uint64_t;

struct Socket
{
    ip_t ip;
    port_t port;

    bool operator<(const Socket& r) const;
};

std::ostream& operator<<(std::ostream& out, const Socket& s);

struct ProtocolData
{
    Socket src;
    Socket dst;
};

std::ostream& operator<<(std::ostream& out, const ProtocolData& s);

struct SocketTraffic
{
    std::map<Socket, counter_t> per_socket_count;
    counter_t total_count;
};

std::ostream& operator<<(std::ostream& out, const SocketTraffic& s);

struct PortTraffic
{
    time_t observation_time;
    std::map<port_t, SocketTraffic> amount_per_dest_port{};
    counter_t total_count;

    PortTraffic& operator+=(const ProtocolData& pt);
};

std::ostream& operator<<(std::ostream& out, const PortTraffic& s);

}
