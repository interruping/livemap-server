
//#include "server_service_logic.hpp"
#include "server_base.hpp"
#include "tcp_server.hpp"
#include "echo_service_logic.hpp"
#include "command_binder.hpp"
#include "client_node_pool.hpp"
int main() {
    using namespace solarcode::livemap;
//
//    server_base *server = new ssl_tcp_server();
//    server->set_delegate(new echo_service_logic());
//    
//    server->start_service();
//    delete server;
    client_node_pool pool;
    command_bind(pool, nullptr, 0, nullptr);
    return 0;
}
