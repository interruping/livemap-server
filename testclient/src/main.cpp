#include <iostream>
#include "solarcode_client.hpp"

int main(int argc, char* argv[])
{
    using namespace solarcode::livemap;
      
    try
    {
        if (argc != 3)
        {
        std::cerr << "Usage: client <host> <port>\n";
        return 1;
    }

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query("localhost", "1212");
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
        ctx.load_verify_file("server.crt");

        client c(io_service, ctx, iterator);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}
