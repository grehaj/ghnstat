#pragma once

#include <optional>
#include <string>
#include <vector>

namespace scan_detector
{
namespace utils
{

class SystemCommand
{
private:
    std::FILE* f{};
    pid_t pid = -1;

    void exec(const std::string& cmd) const;

public:
    SystemCommand(const std::string& cmd);
    SystemCommand(const SystemCommand&) = delete;
    SystemCommand& operator=(const SystemCommand&) = delete;
    SystemCommand(SystemCommand&&) = delete;
    SystemCommand& operator=(SystemCommand&&) = delete;
    ~SystemCommand();

    std::optional<std::string> get_next_output_line();
};

std::vector<std::string> get_active_interfaces_ip();

}
}
