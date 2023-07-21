#include "Utils.h"
#include "Exceptions.h"
#include <algorithm>
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

namespace scan_detector
{
namespace utils
{

constexpr int RSIZE{1024};

SystemCommand::SystemCommand(const std::string& cmd)
{
    int pfd[2]{};
    if (pipe(pfd) == -1)
        throw error::SystemCommandError{"SystemCommand: Unable create pipe."};

    switch (pid = fork())
    {
        case -1:
            throw error::SystemCommandError{"SystemCommand: fork"};
        case 0:
            if (close(pfd[0]) == -1)
                throw error::SystemCommandError{"SystemCommand: sub-process not able to close read end of the pipe."};
            if (pfd[1] != STDOUT_FILENO)
            {
                if (dup2(pfd[1], STDOUT_FILENO) == -1)
                    throw error::SystemCommandError{"SystemCommand: sub-process not able to duplicate STDOUT"};
                if (close(pfd[1]) == -1)
                    throw error::SystemCommandError{"SystemCommand: sub-process not able to close write end of the pipe."};

                exec(cmd);
            }
            default:
                break;
        }
        if (close(pfd[1]) == -1)
            throw error::SystemCommandError{"SystemCommand: sub-process not able to close write end of the pipe."};

        f = fdopen(pfd[0], "r");
        if(f == nullptr)
            throw error::SystemCommandError{"SystemCommand: unable to create output stream"};
}

SystemCommand::~SystemCommand()
{
// TODO cleanup
// kill sub-process if still alive, close all streams , file descriptors
}

void SystemCommand::exec(const std::string& cmd) const
{
    std::istringstream ss{cmd};
    std::vector<std::string> args;

    std::copy(std::istream_iterator<std::string>(ss), {}, std::back_inserter(args));
    if(args.empty())
        throw error::SystemCommandError{"SystemCommand: unable to extract command arguments"};

    std::vector<char*> argv(args.size());
    std::transform(args.begin(), args.end(), argv.begin(), [](const auto& arg) {return const_cast<char*>(arg.c_str());});
    argv.push_back(nullptr);

    execvp(argv[0], argv.data());
    throw error::SystemCommandError{"SystemCommand: execvp"};
}

std::optional<std::string> SystemCommand::get_next_output_line()
{
    char buffer[RSIZE]{};
    if (fgets(buffer, RSIZE, f) != NULL)
        return std::string{buffer};

    return {};
}

std::map<std::string, std::string> get_active_interfaces_ip()
{
    SystemCommand cmd{"ip -4 addr"};
    std::regex r{R"(inet\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))"};
    std::map<std::string, std::string> active_interfaces_ip;
    std::optional<std::string> s;

    while ((s = cmd.get_next_output_line()).has_value())
    {
        std::smatch smtch;
        if(std::regex_search(*s, smtch, r))
        {
            auto start = s->find_last_of(" ") + 1;
            auto ifc_name = s->substr(start);
            if(ifc_name.back() == '\n')
                ifc_name.erase(ifc_name.size() - 1);
            active_interfaces_ip[ifc_name] = smtch[1];
        }
    }

    return active_interfaces_ip;
}

}
}
