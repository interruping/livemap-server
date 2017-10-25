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
 @file solarcode_livemap_node_database.cpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Mar 24, 2017
 @version
*/
#include "client_node_pool.hpp"

namespace solarcode {
namespace livemap {
	/*! @breif 노드 정보를 등록하는 구현*/
	void client_node_pool::register_node(std::weak_ptr<client_node> new_node)
	{
        
        std::shared_ptr<client_node> s_new_node = new_node.lock();
        
		auto rst = _node_container.insert({s_new_node->get_id(), new_node});

		if ( rst.second ){
		//노드 업데이트 성공
		} else {
		//노드 중복 등록시 이전 등록 노드 삭제
            delete_node(s_new_node->get_id());
			_node_container.insert({s_new_node->get_id(), new_node});
		}
	}
	/*! @breif 특정 노드를 삭제하는 구현*/
    void client_node_pool::delete_node(common_id_type delete_node_id)
	{
        _node_container.erase(delete_node_id);
        _node_msg_pool.erase(delete_node_id);
	}
	/*! @breif 등록된 모든 노드를 한 번 씩 참조하는 구현*/
	void client_node_pool::scan_all_nodes(std::function<void(std::weak_ptr<const client_node>)> callback)
	{
		for(auto iter = _node_container.begin(); iter != _node_container.cend(); ++iter ){

            std::weak_ptr<client_node> tmp_ptr = iter->second;
            callback(std::const_pointer_cast<const client_node>(tmp_ptr.lock()));
		}
	}
    
    void client_node_pool::save_msg(int sender_id, int receiver_id, std::string msg)
    {
        auto iter = _node_msg_pool.find(receiver_id);
        if ( iter ==  _node_msg_pool.cend() ) {
            std::queue<std::pair<int, std::string>> msg_queue;
            msg_queue.push(std::make_pair(sender_id, msg));
            _node_msg_pool.insert(std::make_pair(receiver_id, msg_queue));
        } else {
            iter->second.push(std::make_pair(sender_id, msg));
        }
    }
    
    bool client_node_pool::msg_check(int id)
    {
        auto search = _node_msg_pool.find(id);
        
        if ( search != _node_msg_pool.end() ) {
            return true;
        } else {
            return false;
        }
    }
    
    std::pair<int, std::string> client_node_pool::get_msg(int id)
    {
        
        auto search = _node_msg_pool.find(id);

        int sender_id = search->second.front().first;
        std::string msg = search->second.front().second;
        search->second.pop();
        
        if ( search->second.size() == 0 ) {
            _node_msg_pool.erase(id);
        }
        return std::make_pair(sender_id, msg);
    }
}
}

