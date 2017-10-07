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
//  dl_livemap_service_io_manager_boost_impl.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#ifndef dl_livemap_service_io_manager_boost_impl_h
#define dl_livemap_service_io_manager_boost_impl_h

#include "livemapserver_config.h"

#include <condition_variable>
#include <mutex>

#include "boost_asio_helper.hpp"
#include "session_io_manager_base.hpp"
#include "client_node_pool.hpp"

namespace solarcode {
namespace livemap {
    class dl_livemap_service_io_manager_boost_impl : public session_io_manager_base {
    public:
        dl_livemap_service_io_manager_boost_impl(std::weak_ptr<session_base> session, client_node_pool& node_pool);
        
        ~dl_livemap_service_io_manager_boost_impl(){}

        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length);
        virtual std::size_t session_write_before_buffer(char ** buffer);
    private:
        static boost::asio::strand _strand_for_node_pool;
        
        std::mutex _mutex_for_buffer;
        
        std::condition_variable _cv_for_buffer;
        boost::asio::io_service& _io_service;
        
        client_node_pool& _node_pool;
        
        char *_buffer;
        std::size_t _buffer_size;
    };
}
}

#endif /* dl_livemap_service_io_manager_boost_impl_h */
