#include "get.h"

//https://www.boost.org/doc/libs/master/libs/beast/doc/html/beast/quick_start/http_client.html
#include "boost/beast/core.hpp"
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

namespace beast = boost::beast;     // из <boost/beast.hpp>
namespace http = beast::http;       // из <boost/beast/http.hpp>
namespace net = boost::asio;        // из <boost/asio.hpp>
using tcp = net::ip::tcp;           // из <boost/asio/ip/tcp.hpp>

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
    /// <param name="url">веб-адрес ресурса, порт, страница, версия С++</param>
    std::string Rest::Get(std::string host, std::string port, std::string target, int version)
    {
        std::stringstream ss;
        try
        {
            // io_context нужен для всех I/O (ввод/вывод) операций
            net::io_context ioc;

            // объекты для выполнения I/O
            tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc);

            // найти доменное имя
            auto const results = resolver.resolve(host, port);

            // установить соединение для IP адреса, полученного из resolve
            stream.connect(results);

            // подготовить HTTP GET запрос
            // тип GET
            http::request<http::string_body> req{ http::verb::get, target, version };
            // веб-адрес
            req.set(http::field::host, host);
            // user_agent
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

            // послать HTTP запрос веб-ресурсу
            http::write(stream, req);

            // Этот буфер используется для чтения и должен быть сохранен
            beast::flat_buffer buffer;

            // объявить контейнер для хранения ответа
            http::response<http::dynamic_body> res;

            // получить HTTP ответ
            http::read(stream, buffer, res);

            // записать ответ в память
            ss << res;

            // аккуратно закрыть сокет
            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            // not_connected иногда случается
            // так что не утруждайте себя сообщением об этом.
            //
            if (ec && ec != beast::errc::not_connected)
                throw beast::system_error{ ec };

            // если мы здесь, то соединение изящно закрыто
        }
        catch (std::exception const& e)
        {   // ответ = сообщение об ошибке
            ss << "Ошибка: " << e.what() << std::endl;
            //return EXIT_FAILURE;
        }
        return ss.str(); //EXIT_SUCCESS;
    }
}
