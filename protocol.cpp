
#include <iomanip>
#include <iostream>

#include "protocol.hpp"
using asio::ip::tcp;

namespace adb::protocol
{
    /// Encoded the ADB host request.
    /**
     * @param body Body of the request.
     * @return Encoded request.
     */
     static inline std::string host_request(const std::string_view body)
    {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(4) << std::hex << body.size() << body;
        return ss.str();
    }

     static inline bool host_response(tcp::socket& socket, std::string& failure)
    {
        std::array<char, 4> header;
        socket.read_some(asio::buffer(header));
        const auto result = std::string_view(header.data(), 4);
        if (result == "OKAY") {
            return true;
        }

        if (result != "FAIL") {
            failure = "unknown response";
            return false;
        }

        failure = host_message(socket);
        return false;
    }

    std::string host_message(tcp::socket& socket)
    {
        std::array<char, 4> header;
        socket.read_some(asio::buffer(header));
        auto remain = std::stoull(std::string(header.data(), 4), nullptr, 16);

        std::string message;
        std::array<char, 1024> buffer;
        while (remain > 0) {
            const auto length = socket.read_some(asio::buffer(buffer));
            message.append(buffer.data(), length);
            remain -= length;
        }

        return message;
    }

    void send_host_request(tcp::socket& socket, const std::string_view request)
    {
        socket.write_some(asio::buffer(host_request(request)));
        std::string failure;
        if (!host_response(socket, failure)) {
            throw std::runtime_error(failure);
        }
    }

    
}