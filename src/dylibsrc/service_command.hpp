





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
    template <int TYPE_ID>
    class update_node_base : public command_form_base_t<TYPE_ID> {
    public:
    	/*
    	@breif 생성자
    	@param update_info_node 업데이트할 노드 정보를 담은 노드 객체
    	 */
    	update_node_base(client_node update_info):
    	command_form_base_t<TYPE_ID>(),
        _id_seg_info(0,4),
        _lat_seg_info(4,8),
        _lon_seg_info(12,20)
    	{
            auto id = update_info.get_id();
            _id_seg_info = command_form_base_t<TYPE_ID>::add_segment(&id, 4);
            auto coordinate = update_info.getCoordinate();
            
            double latitude = coordinate.latitude;
            _lat_seg_info = command_form_base_t<TYPE_ID>::add_segment(&latitude, 8);
            double longitude = coordinate.longitude;
            _lon_seg_info = command_form_base_t<TYPE_ID>::add_segment(&longitude, 8);
            
    	}
        
        update_node_base(const char * const input_data, const std::size_t input_data_size)
        :command_form_base_t<TYPE_ID>(input_data, input_data_size),
        _id_seg_info(0,4),
        _lat_seg_info(4,8),
        _lon_seg_info(12,20)
        {
            
        }
        
        client_node get_client_node_for_update() const {
            
            int read_id = command_form_base_t<TYPE_ID>::read_segment(_id_seg_info);
            client_node client_node_for_update(read_id);
            double read_lat = command_form_base_t<TYPE_ID>::read_segment(_lat_seg_info);
            double read_lon = command_form_base_t<TYPE_ID>::read_segment(_lon_seg_info);
            client_node_for_update.set_coordinate(make_coordinate(read_lat, read_lon));
            
            return client_node_for_update;
        }
    	virtual ~update_node_base(){}
        
        
        

    private:
        typename command_form_base_t<TYPE_ID>::segment_info _id_seg_info;
        typename command_form_base_t<TYPE_ID>::segment_info _lat_seg_info;
        typename command_form_base_t<TYPE_ID>::segment_info _lon_seg_info;
    };
    
    class user_update_node : public update_node_base<1>
    {
        user_update_node(client_node user_update_info)
        :update_node_base(user_update_info)
        {
            
        }
        
        user_update_node(const char * const input_data, const std::size_t input_data_size)
        :update_node_base(input_data, input_data_size)
        {
            
        }
        
        virtual ~user_update_node () {}
    };
    
    
    class request_user_info : public command_form_base_t<2>
    {
    public:
        request_user_info():
        command_form_base_t () {
            
        }
        
    };
    
    class set_user_info : public command_form_base_t<3>
    {
    public:
        set_user_info(common_id_type new_id) :
        command_form_base_t (),
        _new_id_segment(0,4) {
            _new_id_segment = add_segment(&new_id, sizeof(new_id));
        }
        
        set_user_info(const char * const input_data, const std::size_t input_data_size)
        :command_form_base_t(input_data, input_data_size),
        _new_id_segment(0,4) {
            
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
