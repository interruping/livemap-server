





#include "command_form.hpp"
#include "client_node_pool.hpp"

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
        _id_seg_info(),
        _pos_seg_info()
    	{
            auto id = update_info.get_id();
            _id_seg_info = add_segment(&id, sizeof(id));
            auto coordinate = update_info.getCoordinate();
            _pos_seg_info = add_segment(&coordinate, sizeof(coordinate));
    	}
    	virtual ~update_node(){}
        
        
        

    private:
        segment_info _id_seg_info;
        segment_info _pos_seg_info;
    };
    
    
    class request_node_info : public command_form_base_t<2>
    {
    public:
        request_node_info():
        command_form_base_t () {
            
        }
        
    };
    
    class set_id : public command_form_base_t<3>
    {
    public:
        set_id(common_id_type new_id) :
        command_form_base_t (),
        _new_id_segment() {
            _new_id_segment = add_segment(&new_id, sizeof(new_id));
        }
        
        common_id_type get_id() {
            common_id_type return_id = 0;
            
            read_segment(&return_id, _new_id_segment);
            
            return return_id;
        }
        
    private:
        segment_info _new_id_segment;
        
    };

}
}
