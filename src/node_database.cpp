/*!
 @file solarcode_livemap_node_database.cpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Mar 24, 2017
 @version
*/
#include "solarcode_livemap_node_database.hpp"

namespace solarcode {
namespace livemap {
	/*! @breif 노드 정보를 등록하는 구현*/
	void livemap_node_database::register_node(common_id_type new_node_id, std::shared_ptr<client_node> new_node)
	{
		auto rst = _node_container.insert({new_node_id, new_node});

		if ( rst.second ){
		//노드 업데이트 성공
		} else {
		//노드 중복 등록시 이전 등록 노드 삭제
			delete_node(new_node_id);
			_node_container.insert({new_node_id, new_node});
		}
	}
	/*! @breif 특정 노드를 삭제하는 구현*/
	void livemap_node_database::delete_node(common_id_type delete_node_id)
	{
			_node_container.erase(delete_node_id);
	}
	/*! @breif 등록된 모든 노드를 한 번 씩 참조하는 구현*/
	void livemap_node_database::scan_all_nodes(std::function<void(std::weak_ptr<const client_node>)> callback)
	{
		for(auto iter = _node_container.begin(); iter != _node_container.cend(); ++iter ){

			auto tmp_ptr = iter->second;
			callback(std::const_pointer_cast<const client_node>(tmp_ptr));
		}
	}
}
}

