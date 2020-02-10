#include <cstdio>
#include <cstring>
#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../spdlog/spdlog.h"
#include "../spdlog/sinks/basic_file_sink.h"

constexpr int PORT = 12345;

auto logger_ = spdlog::basic_logger_mt("basic_logger", "client_log");

void
parse_command(int argc,
              char *argv[],
              std::vector<std::string>& params)
{

    std::string s;
    for (int i = 1; i < argc; ++i) {
        s.clear();
        s.insert(s.end(), argv[i], argv[i] + strlen(argv[i]));
        params.push_back(s);
    }

    params[0].erase(0, 2);
    std::transform(params[0].begin(), params[0].end(), params[0].begin(), ::toupper);

}

bool
check_params(std::vector<std::string>& params)
{
    if (params[0] != "GET"
            && params[0] != "SET"
            && params[0] != "LOAD")
    {
        spdlog::error("Wrong parameters");
        logger_->error("Error inserted wrong parameters");
        logger_->flush();
        return false;
    }

    if ((params[0] == "GET" && params.size() < 2)
            || (params[0] == "SET" && params.size() < 3)
            || (params[0] == "LOAD" && params.size() < 2))
    {
        spdlog::error("Not enough parameters");
        logger_->error("Error not enough parameters");
        logger_->flush();
        return false;
    }
    return true;
}

void
helper()
{
    std::cout << "--get <section.parameter>: invoke GET API\n"
              << "--set <section.parameter> <value>: invoke SET API\n"
              << "--load <filepath>: invoke LOAD API\n"
              << "--help: show this message\n";
}

int main(int argc, char *argv[])
{
    /************************************ PARSE ARGS ***********************************/

    /// Check args number. If < 2 return.
    if (argc < 2) {
        spdlog::error("Not enough parameters");
        logger_->error("Error not enough parameters");
        logger_->flush();
        return -1;
    }

    /// Put all params in a string vector.
    std::vector<std::string> params;
    parse_command(argc, argv, params);

    if (params[0] == "HELP") {
        helper();
        return 0;
    }

    /// Check the vector. If commands are GET or SET or LOAD
    /// and are all well formed ok else return -1.
    if (!check_params(params))
        return -1;

    /// Create a command with all params.
    std::string command;
    for (const auto& e : params)
        command += e + " ";

    /********************************* SOCKET CONFIG ***********************************/

    /// Initialize socket
    int sock_fd = 0;

    sockaddr_in s_addr;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        spdlog::error("Error initializing socket");
        logger_->error("Error initializing socket");
        logger_->flush();
        return -1;
    }

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    s_addr.sin_port = htons(PORT);

    spdlog::info("Socket initialized");

    /******************************** CONNECT AND SEND *********************************/

    if (connect(sock_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
    {
        logger_->info("Error connecting to the server");
        spdlog::error("Error connecting to the server");
        return -1;
    }

    send(sock_fd , (command + "\n").c_str() , command.size() + 1, 0);

    /************************************* RECEIVE *************************************/

    logger_->info(command + " sent to the server");
    logger_->info("Waiting for response");

    char buffer[1024 * 8] {0};
    std::string rec {""};

    while (recv(sock_fd, buffer, sizeof (buffer), 0) > 0)
        rec += std::string(buffer);

    std::cout << rec << std::endl;

    return 0;
}
