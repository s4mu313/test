#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <unistd.h>

#include <map>
#include <functional>

#include "thread_pool.h"
#include "support.h"
#include "req_res.h"

#include "../../spdlog/spdlog.h"
#include "../../spdlog/sinks/basic_file_sink.h"

class Web_server {

public:

    using Functions = std::map<std::string, std::function<void(Request, Response)>>;
    using Callback_ = std::function<void(Request, Response)>;

    Web_server(uint16_t port)
        : logger_(spdlog::basic_logger_mt("basic_logger", "log"))
    {
        server_impl::init_server_(sock_fd_, s_addr_, port);
        spdlog::info("Server initialized");
        logger_->info("Server initialized");
        logger_->flush();
    }

    ~Web_server()
    {
        close(sock_fd_);
        spdlog::info("Closing server...");
        logger_->info("Server is shutting down");
        logger_->flush();
    }

    void
    listen()
    {
        spdlog::info("Start listen to the channel");
        logger_->info("Start listen on channel");
        logger_->flush();
        int client;
        while((client = server_impl::accept_(sock_fd_)) > 0)
            tp_.exec(_callback, client, paths_f_, logger_);
    }

    Web_server&
    on_command(std::string path,
               Callback_&& f)
    {
        paths_f_[path] = f;
        return *this;
    }


    Web_server&
    on_default(Callback_&& f)
    {
        paths_f_["default"] = f;
        return *this;
    }

private:

    Thread_pool tp_;
    Functions paths_f_;

    int sock_fd_;
    sockaddr_in s_addr_;

    std::shared_ptr<spdlog::logger> logger_;


    static void _callback(int client, const Functions& paths_f, std::shared_ptr<spdlog::logger> logger_) {

        std::string req, command, data;
        server_impl::receive_(client, req);

        if (server_impl::check_format_(req)) {

            command = req.substr(0, req.find(' '));
            data = req.substr(req.find(' '));

            server_impl::trim_(command);
            server_impl::trim_(data);

            if (command.size() == 0 || data.size() == 0 || paths_f.find(command) == paths_f.end()) {
                paths_f.at("default")({command, data}, {client});
                spdlog::info("Unknown command received");
                logger_->info("Unknown command " + command);
                logger_->flush();
            } else {
                paths_f.at(command)({command, data}, {client});
                spdlog::info("Received command " + command);
                logger_->info("Received command " + command);
                logger_->flush();
            }

        } else {
            paths_f.at("default")({command, data}, {client});
            spdlog::info("Unknown command received");
            logger_->info("Unknown command " + command);
            logger_->flush();
        }

        close(client);
    }

};

#endif // WEB_SERVER_H
