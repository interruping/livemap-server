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
 @file solarcode_livemap_node_database.hpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Mar 23, 2017
 @version
*/

#ifndef SRC_HEADER_SOLARCODE_LIVEMAP_NODE_DATABASE_HPP_
#define SRC_HEADER_SOLARCODE_LIVEMAP_NODE_DATABASE_HPP_
#include <memory>
#include <functional>
#include <mutex>

#include "id_manager.hpp"
#include "client_node.hpp"


namespace solarcode {
namespace livemap {
	class client_node_pool {
	public:
		/*!
		 @breif 생성자
		 */
		client_node_pool()
		: _node_container()
        , _node_msg_pool()
        {

		}
		virtual ~client_node_pool(){}

		virtual void register_node(std::weak_ptr<client_node> new_node);
		virtual void delete_node(common_id_type delete_node_id);
		virtual void scan_all_nodes(std::function<void(std::weak_ptr<const client_node>)> callback);
        virtual void save_msg(int sender_id, int receiver_id, std::string msg);
        virtual bool msg_check(int id);
        virtual std::pair<int, std::string> get_msg(int id);
        
	private:
		std::map<int, std::weak_ptr<client_node>> _node_container;
        std::map<int, std::pair<int,std::string>> _node_msg_pool;
	};
}
}


#endif /* SRC_HEADER_SOLARCODE_LIVEMAP_NODE_DATABASE_HPP_ */
