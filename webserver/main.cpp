#include <iostream>

#include "include/web_server.h"

#include <ini_parser.h>

int main()
{

    Web_server s(12345);

    Ini_parser p;

    s.on_command("GET", [&](Request req, Response res) {

        std::string value;
        res.send(std::to_string(p.get_value(req.data.substr(0, req.data.find('\n')), value))
                 + " " + value + "\n");

    }).on_command("SET", [&](Request req, Response res) {

        std::string value = req.data.substr(req.data.find(' '));

        res.send(std::to_string(p.set_value(req.data.substr(0, req.data.find(' ')), value)) + "\n");

    }).on_command("LOAD", [&](Request req, Response res) {

        res.send(std::to_string(p.load_resource(req.data.substr(0, req.data.find('\n')))) + "\n");

    }).on_default([](Request, Response res){

        res.send("127\n");

    }).listen();

    return 0;
}
