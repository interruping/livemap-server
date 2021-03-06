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
@file solarcode_command_binder.hpp
@breif 버퍼를 분석해 명령으로 변환해주는
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 03. 09.
@version 0.0.1
*/

#ifndef SRC_HEADER_SOLARCODE_COMMAND_BINDER_HPP_
#define SRC_HEADER_SOLARCODE_COMMAND_BINDER_HPP_

#include "livemapserver_config.h"

#include "client_node_pool.hpp"

#include <vector>

namespace solarcode {
namespace livemap {

	static const char * const COMMAND_BINDER_LIB_PATH
#ifdef __APPLE__
    = "./dylib/libcommandbinder.dylib";
#elif __linux
    = "./dylib/libcommandbinder.so";
#elif __unix
    = "./dylib/libcommandbinder.so";
#endif
    
	static const char * const GO_BIND_COMMAND_SYMBOL = "do_command_bind";

    using command_binder_type = std::size_t(*)(std::weak_ptr<client_node> ,
                                               client_node_pool& ,
                                               char *const ,
                                               const std::size_t ,
                                               char**);

	void calc_file_sha1_hash(const char * const file_path, char *const hash_buffer);


	bool compare_sha1_hash(const char * const first_sha1, const char * const second_sha1 );

	command_binder_type get_command_binder(void** handle);

    std::size_t command_bind(std::weak_ptr<client_node> request_node,
                             client_node_pool& node_db,
                             char * const raw_query,
                             const std::size_t raw_query_size,
                             char **result_buffer);
}
}




#endif /* SRC_HEADER_SOLARCODE_COMMAND_BINDER_HPP_ */
