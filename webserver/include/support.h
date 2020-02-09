#ifndef SUPPORT_H
#define SUPPORT_H

#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <string>
#include <stdexcept>

namespace server_impl {

/**
 * @brief Right trim.
 * @param str
 */
void
rtrim_(std::string &str)
{
    str.erase(std::find_if_not(str.rbegin(),
                               str.rend(),
                               [](unsigned char c) { return std::isspace(c); }).base(),
              str.end());
}

/**
 * @brief Left trim.
 * @param str
 */
void
ltrim_(std::string &str)
{
    str.erase(str.begin(),
              std::find_if_not(str.begin(),
                               str.end(),
                               [](unsigned char c) { return std::isspace(c); }));
}

/**
 * @brief Left and right trim.
 * @param str
 */
void
trim_(std::string &s)
{
    ltrim_(s);
    rtrim_(s);
}

/**
 * @brief Accept a connection.
 * @param sock_fd
 * @return 0 if ok, -1 if error.
 */
int
accept_(int sock_fd)
{
    sockaddr_in c_addr;
    socklen_t c_length = sizeof(c_addr);
    return accept(sock_fd, reinterpret_cast<sockaddr*>(&c_addr), &c_length);
}
/**
 * @brief Receive data from socket (8192 bytes).
 * @param fd
 */
void
receive_(int fd, std::string& data)
{
    char buffer[1024 * 8] {0};
    while (recv(fd, buffer, sizeof (buffer), MSG_DONTWAIT) > 0)
        data += std::string(buffer);
}

/**
 * @brief Send message to socket.
 * @param fd
 * @param msg
 * @return true if succeded, false otherwise.
 */
bool
send_m_(int fd,
        const std::string& msg)
{
    std::string res = msg;
    return send(fd, res.c_str(), res.size(), MSG_NOSIGNAL) >= 0;
}

/**
 * @brief Check format.
 * @param req
 * @return true if succeded, false otherwise.
 */
bool
check_format_(std::string& req)
{
    if (req.size() == 0 || req[req.size() - 1] != '\n')
        return false;

    req.pop_back();
    trim_(req);

    return req.find(' ') != std::string::npos;
}

/**
 * @brief Bind channel. If error, throw exception.
 * @param sock_fd
 * @param s_addr
 */
void
set_channel_(int sock_fd,
             sockaddr_in& s_addr)
{
    if (bind(sock_fd, reinterpret_cast<sockaddr*>(&s_addr), sizeof(s_addr)) < 0)
        throw std::runtime_error("binding_failed");
    listen(sock_fd, 32);
}

/**
 * @brief Initialize server socket.
 * @param sock_fd
 * @param s_addr
 * @param port
 */
void init_server_(int& sock_fd,
                  sockaddr_in& s_addr,
                  uint16_t port)
{
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // To avoid long waiting when restart program
    const int optVal = 1;
    const socklen_t optLen = sizeof(optVal);
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const void*>(&optVal), optLen);

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    s_addr.sin_port = htons(port);

    set_channel_(sock_fd, s_addr);

}

};

#endif // SUPPORT_H
