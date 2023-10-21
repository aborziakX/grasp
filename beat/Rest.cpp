﻿#include "Rest.h"

//https://www.boost.org/doc/libs/master/libs/beast/doc/html/beast/quick_start/http_client.html
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

namespace Grasp {
    Rest::Rest()
    {
    }

    Rest::~Rest()
    {
    }


    /// <summary>
    /// GET запрос
    /// </summary>
    /// <param name="url">веб-адрес ресурса</param>
    std::string Rest::Get(std::string host, std::string port, std::string target, int version)
    {
        std::stringstream ss;
        try
        {
            // The io_context is required for all I/O
            net::io_context ioc;

            // These objects perform our I/O
            tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc);

            // Look up the domain name
            auto const results = resolver.resolve(host, port);

            // Make the connection on the IP address we get from a lookup
            stream.connect(results);

            // Set up an HTTP GET request message
            http::request<http::string_body> req{ http::verb::get, target, version };
            req.set(http::field::host, host);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

            // Send the HTTP request to the remote host
            http::write(stream, req);

            // This buffer is used for reading and must be persisted
            beast::flat_buffer buffer;

            // Declare a container to hold the response
            http::response<http::dynamic_body> res;

            // Receive the HTTP response
            http::read(stream, buffer, res);

            // Write the message to standard out
            ss << res;

            // Gracefully close the socket
            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            // not_connected happens sometimes
            // so don't bother reporting it.
            //
            if (ec && ec != beast::errc::not_connected)
                throw beast::system_error{ ec };

            // If we get here then the connection is closed gracefully
        }
        catch (std::exception const& e)
        {
            ss << "Error: " << e.what() << std::endl;
            //return EXIT_FAILURE;
        }
        return ss.str(); //EXIT_SUCCESS;
    }

}
