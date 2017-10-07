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
//  dl_livemap_service_io_manager_boost_impl.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#include "dl_livemap_service_io_manager_boost_impl.hpp"
#include "command_binder.hpp"

namespace solarcode {
namespace livemap {
    dl_livemap_service_io_manager_boost_impl::dl_livemap_service_io_manager_boost_impl(std::weak_ptr<session_base> session, client_node_pool& node_pool)
    :solarcode::livemap::session_io_manager_base(session),
    _mutex_for_buffer(),
    _cv_for_buffer(),
    _io_service(get_shared_io_service()),
    _node_pool(node_pool),
    _buffer(nullptr),
    _buffer_size(0)
    {
        
        
        
    }
    
    
    boost::asio::strand
    dl_livemap_service_io_manager_boost_impl::_strand_for_node_pool
    = boost::asio::strand(get_shared_io_service());

    void dl_livemap_service_io_manager_boost_impl::session_read_after_buffer(char *const buffer, const std::size_t buffer_length)
    {

        
        auto unsafe_service = [this, buffer, buffer_length]() {
            std::unique_lock<std::mutex> lock_for_buffer(_mutex_for_buffer);
   
            _buffer_size = command_bind(get_session_owner(),_node_pool, buffer, buffer_length, &_buffer);
            
            delete [] buffer;
            //_cv_for_buffer.notify_one();
            
        };
        
        auto safe_service = _strand_for_node_pool.wrap(unsafe_service);
        _io_service.post(safe_service);
        
    }
    
    std::size_t dl_livemap_service_io_manager_boost_impl::session_write_before_buffer(char **buffer)
    {
        std::unique_lock<std::mutex> lock_for_buffer(_mutex_for_buffer);
        
        *buffer = _buffer;
        

        _buffer = nullptr;
        std::size_t size_to_return = _buffer_size;
        _buffer_size = 0;
        
        return size_to_return;
    }
}
    
}
