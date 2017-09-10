
//#include "server_service_logic.hpp"

#include "tcp_server.hpp"
#include "echo_service_logic.hpp"

#include "client_node_pool.hpp"
#include "command_form.hpp"
#include "command_binder.hpp"

int main() {
    using namespace solarcode::livemap;

    server_base *server = new ssl_tcp_server();
    server->set_delegate(new echo_service_logic());
    
    server->start_service();
    delete server;
    
//    int type = 2;
//    char *raw_request = new char[4];
//    std::memcpy(raw_request, &type, sizeof(type));
//    
//    client_node_pool pool;
//    std::shared_ptr<client_node> client = std::make_shared<client_node>(1);
//    
//    char *result = nullptr;
//    auto size = command_bind(client, pool, raw_request, sizeof(type), &result);
//    
//    
    
    return 0;
}
