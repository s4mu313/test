#ifndef REQ_RES_H
#define REQ_RES_H

#include <string>
#include <vector>

#include "support.h"

struct Request
{
    std::string command;
    std::string data;

    Request(std::string cmd,
            std::string cmd_data)
        : command(cmd),
          data(cmd_data)
    {}
};

struct Response
{
    int client;

    Response(int clt)
        : client(clt)
    {}

    bool
    send(std::string msg) const
    { return server_impl::send_m_(client, msg); }

};

void
operator>>(const Response& res,
           const std::string& msg)
{ res.send(msg); }


#endif // REQ_RES_H
