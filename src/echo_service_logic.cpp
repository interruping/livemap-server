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
    
    void echo_service_logic::error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error)
    {
        //do nothing...
        socket = socket;
        error = error;
    }

}
}
