#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <print>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;
using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;

awaitable<void> handle_session( tcp::socket socket )
{
    try
    {
        auto endpoint = socket.remote_endpoint();
        std::string client_ip = endpoint.address().to_string();
        std::cout << "New connection from: " << client_ip << "\n";

        beast::flat_buffer buffer;
        http::request<http::string_body> request;

        // Read request
        co_await http::async_read( socket, buffer, request, use_awaitable );

        http::response<http::string_body> response( http::status::ok, request.version() );
        response.set( http::field::server, "Async-Beast" );
        response.set( http::field::content_type, "text/plain" );

        if ( request.method() != http::verb::get )
        {
            response.result( http::status::bad_request );
            response.body() = "Only GET supported";
        }
        else
        {
            std::string path = std::string( request.target() );

            if ( path == "/" )
            {
                response.body() = "Welcome to the root endpoint!";
            }
            else if ( path == "/status" )
            {
                response.body() = "Status: OK";
            }
            else if ( path == "/api/info" )
            {
                response.body() = "Info: version 1.0";
            }
            else
            {
                response.result( http::status::not_found );
                response.body() = "404 Not Found";
            }
        }

        response.prepare_payload();
        co_await http::async_write( socket, response, use_awaitable );

        beast::error_code ec;
        socket.shutdown( tcp::socket::shutdown_send, ec );
    }
    catch ( std::exception& e )
    {
        std::cerr << "Session error: " << e.what() << "\n";
    }
}

awaitable<void> listener( uint16_t port )
{
    auto executor = co_await asio::this_coro::executor;
    tcp::acceptor acceptor( executor, { tcp::v4(), port } );
    std::cout << "Listening on http://127.0.0.1:" << port << "\n";

    for ( ;;)
    {
        tcp::socket socket = co_await acceptor.async_accept( use_awaitable );
        co_spawn( executor, handle_session( std::move( socket ) ), detached );
    }
}

int main()
{
    try
    {
        asio::io_context io;
        co_spawn( io, listener( 8080 ), detached );
        io.run();
    }
    catch ( const std::exception& e )
    {
        std::cerr << "Fatal: " << e.what() << "\n";
    }
}
