
//#include "server_service_logic.hpp"

#include "tcp_server.hpp"
#include "livemap_service_logic.hpp"

int main() {
   using namespace solarcode::livemap;

    server_base *server = new ssl_tcp_server();
    server->set_delegate(new livemap_service_logic());
    
    server->start_service();
    delete server;


    return 0;
}
