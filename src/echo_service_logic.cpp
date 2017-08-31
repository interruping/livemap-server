//
//  echo_service_logic.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 30..
//
//

#include "echo_service_logic.hpp"
#include "session_base.hpp"
#include "repeat_io_manager.hpp"

namespace solarcode {
namespace livemap {
    echo_service_logic::echo_service_logic()
    :_session_builder()
    {
        
    }
    
    void echo_service_logic::set_target_server(server_base *target_server) {
        _session_builder = target_server->get_session_builder();
    }
    
    void echo_service_logic::handle_accept(std::shared_ptr<void> socket){
        std::shared_ptr<session_base> new_session = _session_builder(socket);
        
        new repeat_io_manager(new_session);
        new_session->start();
    }
    
    void echo_service_logic::error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error){
        
    }

}
}
