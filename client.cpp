#include <asio.hpp>
#include <fstream>
#include <iostream>
#include <thread>

#include "client.hpp"
#include "protocol.hpp"
using asio::ip::tcp;

namespace adb
{
    static inline std::string version(asio::io_context& content, const tcp::resolver::results_type& endpoints)
    {
        tcp::socket socket(content);
        asio::connect(socket,endpoints);

        const auto request = "host:version";
        adb::protocol::send_host_request(socket, request);

        return protocol::host_message(socket);
    }

    std::string version()
    {
        asio::io_context content;
        asio::ip::tcp::resolver resolver(content);
        const auto endpoint = resolver.resolve("127.0.0.1","7890");
        return version(content,endpoint);
    }

    std::string devices()
    {
        
        return "";
    }

    void kill_server()
    {
        return;
    }
}