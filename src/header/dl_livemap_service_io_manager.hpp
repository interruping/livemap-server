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
//  dl_livemap_service_io_manager.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#ifndef dl_livemap_service_io_manager_h
#define dl_livemap_service_io_manager_h

#include "livemapserver_config.h"

#include "session_io_manager_base.hpp"
#include "client_node_pool.hpp"
#include "debug_utility.hpp"

namespace solarcode {
namespace livemap {
    class dl_livemap_service_io_manager : public session_io_manager_base {
    public:
        dl_livemap_service_io_manager(std::weak_ptr<session_base> session, client_node_pool& node_pool);
        
        virtual ~dl_livemap_service_io_manager() {
#ifdef _DEBUG_
            SC_DBGMSG("dl_livemap_service_io_manager destructor execute");
#endif
        }
        
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length);
        virtual std::size_t session_write_before_buffer(char ** buffer);
        
        virtual void set_session_owner(std::shared_ptr<client_node> owner);
        virtual std::weak_ptr<client_node> get_session_owner() const;
        
        virtual std::weak_ptr<session_base> get_session() const;
    private:
        std::unique_ptr<session_io_manager_base> _pimple;
    };
}
}


#endif /* dl_livemap_service_io_manager_h */
