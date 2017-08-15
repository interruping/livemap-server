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

#include "solarcode_id_manager.hpp"
#include "solarcode_livemap_client_node.hpp"

namespace solarcode {
namespace livemap {
	template <typename node_id_type, typename node_type>
	class node_database {
	public:
		/*!
		 @breif 생성자
		 */
		node_database(){

		}
		/*!
		 @breif 소멸자
		 */
		virtual ~node_database(){}

		/*!
		 @brief 노드 등록 함수
		 @param new_node_id 등록할 노드 id
		 @param new_node 등록할 노드
		 */
		virtual void register_node(node_id_type new_node_id, std::shared_ptr<node_type> new_node) = 0;

		/*!
		 @breif 노드 삭제 함수

		 */
		virtual void delete_node(node_id_type delete_node_id)  = 0;
		/*!
		 @breif 모든 노드를 스캔한다.
		 @param 노드를 스캔할때 마다 호출되는 콜백
		 */
		virtual void scan_all_nodes(std::function<void(std::weak_ptr<const node_type>)> callback) = 0;


	};
}
}

namespace solarcode {
namespace livemap {
	//라이브맵 노드 데이터 베이스 베이스 클래스 alias
	using livemap_node_database_base = node_database<common_id_type, client_node>;
	class livemap_node_database : public livemap_node_database_base {
	public:
		/*!
		 @breif 생성자
		 */
		livemap_node_database()
		:_node_container(){

		}
		virtual ~livemap_node_database(){}

		virtual void register_node(common_id_type new_node_id, std::shared_ptr<client_node> new_node);
		virtual void delete_node(common_id_type delete_node_id);
		virtual void scan_all_nodes(std::function<void(std::weak_ptr<const client_node>)> callback);
	private:
		std::map<common_id_type, std::shared_ptr<client_node>> _node_container;
	};
}
}


#endif /* SRC_HEADER_SOLARCODE_LIVEMAP_NODE_DATABASE_HPP_ */
