#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <thread>

namespace beast = boost::beast; // из <boost/beast.hpp>
namespace http = beast::http;   // из <boost/beast/http.hpp>
namespace net = boost::asio;    // из <boost/asio.hpp>
using tcp = net::ip::tcp;       // из <boost/asio/ip/tcp.hpp>

// Сообщить об ошибке
void
fail(beast::error_code ec, char const* err)
{
    std::cerr << err << ": " << ec.message() << "\n";
}

// HTTP-ответ на запрос.
http::response<http::string_body> handle_request(http::request<http::string_body> const& req) {
    std::cout << "handle_request" << std::endl;
    std::cout << "req:\n" << req << std::endl;
    std::cout << "req.method() = " << req.method() << std::endl;
    std::cout << "req.target() = " << req.target() << std::endl;
    if (req.method() == http::verb::get) {
        // Обработка GET запроса
        std::cout<<"Обработка GET запроса"<<std::endl;
        nlohmann::json json_response = {{"message", "Это GET запрос"}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    } else if (req.method() == http::verb::post) {
        // Обработка POST запроса
        std::cout<<"Обработка POST запроса"<<std::endl;
        // Извлечение содержимого файла
	auto body = req.body();    
    	// Сохранение файла
    	std::ofstream ofs("uploaded_file.txt", std::ios::out | std::ios::binary);
    	if (ofs.is_open()) {
        	ofs << body;
        	ofs.close();
        	http::response<http::string_body> res1(http::status::ok, req.version());
        	res1.set(http::field::server, "Boost.Beast");
        	res1.set(http::field::content_type, "application/json");       	
        	
        	auto json_request1 = nlohmann::json::parse(req.body());        	        	
	        
	        nlohmann::json json_response1 = {{"message", json_request1.dump()}, {"result", "File uploaded successfully"}};
        	
        	res1.body() = json_response1.dump() + "\n";
        	res1.prepare_payload();
        	return res1;
        }
        
        auto json_request = nlohmann::json::parse(req.body());
        std::string response_message = "Получено: " + json_request.dump();
        nlohmann::json json_response = {{"message", response_message}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    } else if (req.method() == http::verb::delete_) {
        // Обработка DELETE запроса
        std::cout<<"Обработка DELETE запроса"<<std::endl;
        nlohmann::json json_response = {{"message", "Ресурс удален"}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }

    std::cout << "-----------------------------------------------------" << std::endl;
    // Ответ, если метод не является GET, POST или DELETE методом
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}

// Обработка HTTP подключений – чтение, запись данных.
class Session : public std::enable_shared_from_this<Session> {
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;

public:
    explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void run() {
        std::cout << "Start run() from Session class" << "\n";
        do_read();
    }

private:
    void do_read() {
    	std::cout<<"do_read()"<<std::endl;
        auto self(shared_from_this());
        http::async_read(socket_, buffer_, req_, [this, self](beast::error_code ec, std::size_t) {
            if (!ec) {            	
            	   std::cout << "req_:\n" << req_ << std::endl;
                do_write(handle_request(req_));
            }
        });
    }

    void do_write(http::response<http::string_body> res) {
        auto self(shared_from_this());
        auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
        http::async_write(socket_, *sp, [this, self, sp](beast::error_code ec, std::size_t) {
            socket_.shutdown(tcp::socket::shutdown_send, ec);
        });
    }
};

// Принимаются входящие подключения, запускаются сеансы..
class Listener : public std::enable_shared_from_this<Listener> {
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint)
        : ioc_(ioc), acceptor_(net::make_strand(ioc)) {
        beast::error_code ec;

        // Открываем сокет с заданным IP-протоколом
        acceptor_.open(endpoint.protocol(), ec);
        if (ec) {
            std::cerr << "Ошибка открытия: " << ec.message() << std::endl;
            return;
        }

        // Разрешение повторного использования адреса
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec) {
            std::cerr << "Ошибка установки параметра: " << ec.message() << std::endl;
            return;
        }

        // Связывает сокет с локальной конечной точкой
        acceptor_.bind(endpoint, ec);
        if (ec) {
            std::cerr << "Ошибка привязки: " << ec.message() << std::endl;
            return;
        }

        // Устанавливает объект в состояние прослушивания
        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec) {
            std::cerr << "Ошибка прослушивания: " << ec.message() << std::endl;
            return;
        }        
    }
    void run()
    {
        std::cout << "Start run() from Listener class" << "\n"; 
        net::dispatch(
            acceptor_.get_executor(),
            beast::bind_front_handler(
                &Listener::do_accept,
                this->shared_from_this()));
    }

private:
    void do_accept() {        
        

        // Принять новое соединение
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &Listener::on_accept,
                shared_from_this()));
    }
    void
    on_accept(beast::error_code ec, tcp::socket socket)
    {
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Создать http сессию и запустить ее            
            std::make_shared<Session>(std::move(socket))->run();
        }

        // Принять следующее соединение
        do_accept();
    }
};

int main() {
    try {
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 8080;

        net::io_context ioc{1};

        std::cout << "Listening port: " << port << "\n";

        std::make_shared<Listener>(ioc, tcp::endpoint{address, port})->run();

        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}
