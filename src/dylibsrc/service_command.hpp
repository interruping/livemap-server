





#include "command_form.hpp"
#include "client_node.hpp"

namespace solarcode {
namespace livemap {
    /*!
    @class update_node
    @brief 노드 업데이트 명령 클래
    @detail n/a.
    @namespace solarcode::livemap::update_node;
    @see
    */
    class update_node : public command_form_base_t<1> {
    public:
    	/*
    	@breif 생성자
    	@param update_info_node 업데이트할 노드 정보를 담은 노드 객체
    	 */
    	update_node(client_node update_info):
    	command_form_base_t(),
		_position_update_id_index(0),
		_position_data_index(0)
    	{
    		auto id = update_info.get_id();
    		_position_update_id_index = add_segment(&id, sizeof(id));
    		auto pos = update_info.getCoordinate();
    		_position_data_index = add_segment(&pos ,sizeof(pos));
    	}
    	virtual ~update_node(){}

    private:
    	std::size_t _position_update_id_index;
    	std::size_t _position_data_index;

    };

}
}
