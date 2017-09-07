//
//  livemap_service_logic.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 1..
//
//

#include "livemap_service_logic.hpp"
#include "dl_livemap_service_io_manager.hpp"


namespace solarcode {
namespace livemap {
    
    void livemap_service_logic::set_target_server(server_base *target_server) {
        _session_builder = target_server->get_session_builder();
        
    }
    
    void livemap_service_logic::handle_accept(std::shared_ptr<void> socket) {
        std::shared_ptr<session_base> new_session = _session_builder(socket);
        session_io_manager_base *new_session_io_manager
        = new dl_livemap_service_io_manager(new_session, _node_pool);
        
        std::unique_lock<std::mutex> lock_for_id_manager;
        
        common_id_type new_id = _id_manager.request_id();
        
        lock_for_id_manager.unlock();
        
        if ( new_id == no_more_id ) {
            return;
        }
        
        
        
        std::shared_ptr<client_node> new_client = std::make_shared<client_node>(new_id);
        
        new_session_io_manager->set_session_owner(new_client);
        
        new_session->set_expire_callback([this, new_id]{
            std::unique_lock<std::mutex> lock_for_id_manager;
            _id_manager.return_id(new_id);
            lock_for_id_manager.unlock();
        });
        
        new_session->start();
        
    }
    
    void livemap_service_logic::error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error) {
        
    }
}
}
