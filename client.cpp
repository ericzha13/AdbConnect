#include <asio.hpp>
#include <fstream>
#include <iostream>
#include <thread>

#include "client.hpp"
#include "protocol.hpp"
using asio::ip::tcp;
using adb::protocol::send_host_request;

namespace adb
{
    static inline std::string version(asio::io_context& content, const tcp::resolver::results_type& endpoints)
    {
        tcp::socket socket(content);
        asio::connect(socket,endpoints);//throw system:error if connect failed.

        const auto request = "host:version";
        adb::protocol::send_host_request(socket, request);

        return protocol::host_message(socket);
    }

    static inline std::string devices(asio::io_context& content, const tcp::resolver::results_type& endpoints)
    {
        tcp::socket socket(content);
        asio::connect(socket,endpoints);

        const auto request = "host:devices";
        send_host_request(socket,request);
        return protocol::host_message(socket);
    }

    std::string version()
    {
        asio::io_context content;
        asio::ip::tcp::resolver resolver(content);
        const auto endpoint = resolver.resolve("127.0.0.1","5037");
        //const auto endpoint = resolver.resolve("192.168.110.184", "5555");
        return version(content,endpoint);
    }

    std::string devices()
    {
        asio::io_context content;
        asio::ip::tcp::resolver resolver(content);
        const auto endpoint = resolver.resolve("127.0.0.1","5037");
        //const auto endpoint = resolver.resolve("192.168.110.184", "5555");
        return devices(content,endpoint);
    }

    void kill_server()
    {
        asio::io_context content;
        tcp::resolver resolver(content);
        const auto endpoint = resolver.resolve("127.0.0.1", "5037");

        tcp::socket socket(content);
        asio::connect(socket,endpoint);

        const auto request = "host:kill";
        send_host_request(socket, request);

        return;
    }


    class client_impl : public client
    {
    public:
        client_impl(const std::string_view serial);
        std::string connect() override;
        std::string disconnect() override;
        std::string version() override;
        std::string devices() override;
        std::string shell(const std::string_view command) override;
        std::string exec(const std::string_view command) override;
        bool push(const std::string_view src, const std::string_view dst, int perm) override;
        std::string root() override;
        std::string unroot() override;
        void wait_for_device() override;

    private:
        friend class client;

        std::string m_serial;
        asio::io_context m_context;
        tcp::resolver::results_type m_endpoints;

        /// Switch the connection to the device.
        /**
         * @param socket Opened adb connection.
         * @note Should be used only by the client class.
         * @note Local services (e.g. shell, push) can be requested after this.
         */
        void switch_to_device(asio::ip::tcp::socket& socket);
    };


    std::shared_ptr<client> client::create(const std::string_view serial)
    {
        return std::make_shared<client_impl>(serial);
    }

    client_impl::client_impl(const std::string_view serial)
    {
        m_serial = serial;

        tcp::resolver resolver(m_context);
        m_endpoints = resolver.resolve("127.0.0.1","5037");
       
    }

    std::string client_impl::connect()
    {
        tcp::socket socket(m_context);
        asio::connect(socket,m_endpoints);

        const auto request = "host:connect:" + m_serial;
        send_host_request(socket, request);

        return protocol::host_message(socket);
    }

    std::string client_impl::disconnect()
    {
        tcp::socket socket(m_context);
        asio::connect(socket, m_endpoints);

        const auto request = "host:disconnect:" + m_serial;
        send_host_request(socket, request);

        return protocol::host_message(socket);
    }

    std::string client_impl::version()
    {
        return adb::version(m_context,m_endpoints);
    }

    std::string client_impl::devices()
    {
        return adb::devices(m_context, m_endpoints);
    }

    std::string client_impl::shell(const std::string_view command)
    {
        
    }

    void client_impl::switch_to_device(asio::ip::tcp::socket& socket)
    {
        const auto request = "host:transport:" + m_serial;
        send_host_request(socket, request);
    }
}