#pragma once

#include <string_view>

#include <asio/ip/tcp.hpp>

namespace adb::protocol
{
    /// Receive encoded data from the host.
    /**
     * @param socket Opened adb connection.
     * @return Data from the host.
     * @throw std::runtime_error Thrown on socket failure.
     */
    std::string host_message(asio::ip::tcp::socket& socket);

    void send_host_request(asio::ip::tcp::socket& socket, const std::string_view request);

    std::string host_message(asio::ip::tcp::socket& socket);
}