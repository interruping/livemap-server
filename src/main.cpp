
//#include "server_service_logic.hpp"
#include "server_base.hpp"
#include "tcp_server.hpp"
#include "echo_service_logic.hpp"

int main() {
  using namespace solarcode::livemap;
//   livemap_service_setting setting;
//   setting.set_option(livemap_server_multi_thread, 2);
//   setting.set_option(livemap_server_service_port, 1212);
//   livemap_server_service_logic server_app(setting);
//
//   server_app.start_service();

    server_base *server = new ssl_tcp_server();
    server->set_delegate(new echo_service_logic());
    
    server->start_service();
    delete server;
    
    return 0;
}
