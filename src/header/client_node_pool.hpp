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
		:_node_container(){

		}
		virtual ~client_node_pool(){}

		virtual void register_node(std::weak_ptr<client_node> new_node);
		virtual void delete_node(common_id_type delete_node_id);
		virtual void scan_all_nodes(std::function<void(std::weak_ptr<const client_node>)> callback);
	private:
		std::map<common_id_type, std::weak_ptr<client_node>> _node_container;
	};
}
}


#endif /* SRC_HEADER_SOLARCODE_LIVEMAP_NODE_DATABASE_HPP_ */
