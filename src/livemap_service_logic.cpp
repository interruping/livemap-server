// Copyright 2017 GeunYoung Lim <interruping4dev@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
        
        
        std::unique_lock<std::mutex> lock_for_id_manager_and_node_pool(_mutex_for_id_manager_and_node_pool);
        
        common_id_type new_id = _id_manager.request_id();
        std::shared_ptr<client_node> new_client = std::make_shared<client_node>(new_id);
        
        _node_pool.register_node(new_client);
#ifdef _DEBUG_
        SC_DBGMSG("new id alloc to session id is " << new_id);
#endif
        lock_for_id_manager_and_node_pool.unlock();
        
        if ( new_id == no_more_id ) {
            return;
        }
    
        
        
        new_session_io_manager->set_session_owner(new_client);

        
        new_session->set_expire_callback([this, new_id]{
            std::unique_lock<std::mutex> lock_for_id_manager_and_node_pool(_mutex_for_id_manager_and_node_pool);
#ifdef _DEBUG_
            SC_DBGMSG("id:" << new_id << " is returned by expired session");
#endif
            _id_manager.return_id(new_id);
            _node_pool.delete_node(new_id);
            lock_for_id_manager_and_node_pool.unlock();
        });
        
        new_session->start();
        
    }
    
    void livemap_service_logic::error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error) {
        
    }
}
}
