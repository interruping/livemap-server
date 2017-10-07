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
//  echo_service_logic.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 30..
//
//

#ifndef echo_service_logic_h
#define echo_service_logic_h

#include "livemapserver_config.h"

#include "service_logic_base.hpp"

#include <memory>

namespace solarcode {
namespace livemap {
    class echo_service_logic : public service_logic_base {
    public:
        echo_service_logic();
        
        virtual ~echo_service_logic() {
            
        }
        
        virtual void set_target_server(server_base *target_server);
        virtual void handle_accept(std::shared_ptr<void> socket);
        virtual void error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error);
    private:
        server_base::session_builder_type _session_builder;
    };
}
}

#endif /* echo_service_logic_h */
