
//#include "server_service_logic.hpp"

#include "tcp_server.hpp"
#include "echo_service_logic.hpp"

#include "command_form.hpp"

int main() {
    using namespace solarcode::livemap;
//
//    server_base *server = new ssl_tcp_server();
//    server->set_delegate(new echo_service_logic());
//    
//    server->start_service();
//    delete server;
    
    command_form_base t1;
    
    const char *first = "this is first";
    
    command_form_base::segment_info s1 =
    t1.add_segment((void *)first, std::strlen(first)+1);
    
    
    const char *second = "second data 123";
    command_form_base::segment_info s2 =
    t1.add_segment((void *)second, std::strlen(second)+1);
    
    int input = 3;
    
    command_form_base::segment_info s3 =
    t1.add_segment(&input, sizeof(input));
    
    command_form_base t2;
    t2 = t1;
    
    int data;
    t2.read_segment(&data, s3);
    
    char serializebuffer[t2.get_entire_size()];
    t2.serialize(serializebuffer);
    
    
    std::cout << data << std::endl;
    
    return 0;
}
