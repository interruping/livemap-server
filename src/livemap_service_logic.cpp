//
//  livemap_service_logic.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 1..
//
//

#include "livemap_service_logic.hpp"

namespace solarcode {
namespace livemap {
    
    void livemap_service_logic::set_target_server(server_base *target_server) {
        _session_builder = target_server->get_session_builder();
        
    }
    
    void livemap_service_logic::handle_accept(std::shared_ptr<void> socket) {
        std::shared_ptr<session_base> new_session = _session_builder(socket);
        
        
    }
    
    void livemap_service_logic::error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error) {
        
    }
}
}
