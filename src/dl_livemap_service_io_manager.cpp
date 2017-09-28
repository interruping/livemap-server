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
//  dl_livemap_service_io_manager.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#include "dl_livemap_service_io_manager.hpp"
#include "dl_livemap_service_io_manager_boost_impl.hpp"

namespace solarcode {
namespace livemap {
    dl_livemap_service_io_manager::dl_livemap_service_io_manager(std::weak_ptr<session_base> session, client_node_pool& node_pool)
    :session_io_manager_base(std::weak_ptr<session_base>()),
    _pimple(new dl_livemap_service_io_manager_boost_impl(session, node_pool))
    {

    }
    void dl_livemap_service_io_manager::session_read_after_buffer(char *const buffer, const std::size_t buffer_length)
    {
        _pimple->session_read_after_buffer(buffer, buffer_length);
    }
    
    std::size_t dl_livemap_service_io_manager::session_write_before_buffer(char ** buffer)
    {
        return _pimple->session_write_before_buffer(buffer);
    }
    
    void dl_livemap_service_io_manager::set_session_owner(std::shared_ptr<client_node> owner) {
        _pimple->set_session_owner(owner);
    }
    
    std::weak_ptr<client_node> dl_livemap_service_io_manager::get_session_owner() const {
        return _pimple->get_session_owner();
    }
    
    std::weak_ptr<session_base> dl_livemap_service_io_manager::get_session() const
    {
        return _pimple->get_session();
    }
    
    
}
}
