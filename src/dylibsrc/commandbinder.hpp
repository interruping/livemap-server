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


/*!
 @file solarcode_livemap_libcommandbinder.hpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Apr 1, 2017
 @version
*/



#ifndef SRC_HEADER_SOLARCODE_LIVEMAP_LIBCOMMANDBINDER_HPP_
#define SRC_HEADER_SOLARCODE_LIVEMAP_LIBCOMMANDBINDER_HPP_

#include "service_command.hpp"


extern "C"{
    std::size_t do_command_bind(std::weak_ptr<solarcode::livemap::client_node> request_node ,
                                solarcode::livemap::client_node_pool& node_db ,
                                char *const raw_request ,
                                const std::size_t raw_request_size ,
                                char **result_buffer );
}
namespace solarcode {
namespace livemap {

    
	command_type read_service_type(const char * const input_buffer);
    
    std::size_t _do_command_bind(std::weak_ptr<client_node> request_node,
                                 client_node_pool& node_db,
                                 char *const raw_request,
                                 const std::size_t raw_request_size,
                                 char **result_buffer);

}
}



#endif /* SRC_HEADER_SOLARCODE_LIVEMAP_LIBCOMMANDBINDER_HPP_ */
