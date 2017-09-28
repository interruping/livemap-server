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

#include "service_type.hpp"
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
        _lon_seg_info(12,8)
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
        _lon_seg_info(12,8)
        {
            
        }
        
        client_node get_client_node_for_update()  {
            
            int read_id = 0;
            command_form_base_t<TYPE_ID>::read_segment(&read_id, _id_seg_info);
            client_node client_node_for_update(read_id);
            
            double read_lat = 0.0f;
            command_form_base_t<TYPE_ID>::read_segment(&read_lat, _lat_seg_info);
            
            double read_lon = 0.0f;
            command_form_base_t<TYPE_ID>::read_segment(&read_lon, _lon_seg_info);
            client_node_for_update.set_coordinate(make_coordinate(read_lat, read_lon));
            
            return client_node_for_update;
        }
    	virtual ~update_node_base(){}
        
        
        

    private:
        typename command_form_base_t<TYPE_ID>::segment_info _id_seg_info;
        typename command_form_base_t<TYPE_ID>::segment_info _lat_seg_info;
        typename command_form_base_t<TYPE_ID>::segment_info _lon_seg_info;
    };
    
    class user_update_node : public update_node_base<USERNODEUPDATE>
    {
    public:
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
    
    
    class request_user_info : public command_form_base_t<REQUESTUSERINFO>
    {
    public:
        request_user_info():
        command_form_base_t () {
            
        }
        
    };
    
    class set_user_info : public command_form_base_t<SETUSERINFO>
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
    
    class near_node_info : public command_form_base_t<NEARNODEINFO>
    {
    public:
        near_node_info()
        :command_form_base_t(),
        _num_of_near_node_info(0,4)
        {
            
        }
        
        near_node_info(const char * const input_data, const std::size_t input_data_size)
        :command_form_base_t(input_data, input_data_size),
        _num_of_near_node_info(0,4)
        {
            
        }
        
        void set_near_node( std::vector<client_node> near_nodes ) {
            if ( get_entire_size() != 0 )
                return;
            
            int count_num_of_near_node = near_nodes.size();
            
            add_segment(&count_num_of_near_node, 4);
            
            for ( auto iter = near_nodes.begin(); iter != near_nodes.end(); ++iter ){
                
                int id = iter->get_id();
                double lat = iter->getCoordinate().latitude;
                double lon = iter->getCoordinate().longitude;
                
                add_segment(&id, 4);
                add_segment(&lat, 8);
                add_segment(&lon, 8);
                
            }
            
        }
        
        std::vector<client_node> get_near_nodes() {
            int read_num_of_near_node_info = 0;
            read_segment(&read_num_of_near_node_info, _num_of_near_node_info);
            
            std::vector<client_node> near_nodes;
            
            const int sizeof_id = 4;
            const int sizeof_lat = 8;
            const int sizeof_lon = 8;
            
            int start_begin = 0;
            
            for ( int index = 0; index < read_num_of_near_node_info; index++ ) {
                segment_info id_seginfo(start_begin, sizeof_id);
                segment_info lat_seginfo(start_begin + sizeof_id, sizeof_lat);
                segment_info lon_seginfo(start_begin + sizeof_id + sizeof_lat, sizeof_lon);
                
                start_begin = sizeof_id + sizeof_lat + sizeof_lon;
                
                int read_id = 0;
                double read_lat = 0.0f;
                double read_lon = 0.0f;
                
                read_segment(&read_id, id_seginfo);
                read_segment(&read_lat, lat_seginfo);
                read_segment(&read_lon, lon_seginfo);
                
                client_node read_node(read_id);
                read_node.set_coordinate(make_coordinate(read_lat, read_lon));
                
                near_nodes.push_back(read_node);
            }
            
            return near_nodes;
        }
        
    private:
        segment_info _num_of_near_node_info;
    };
    
    class user_viewpoint_update : public update_node_base<USERVIEWPOINTUPDATE>
    {
    public:
        user_viewpoint_update(client_node user_update_info, coordinate view_point)
        : update_node_base(user_update_info)
        , _viewpoint_lat(20, 8)
        , _viewpoint_lon(28, 8)
        {
            double lat = view_point.latitude;
            double lon = view_point.longitude;
            add_segment(&lat, 8);
            add_segment(&lon, 8);
        }
        
        user_viewpoint_update(const char * const input_data, const std::size_t input_data_size)
        : update_node_base(input_data, input_data_size)
        , _viewpoint_lat(20, 8)
        , _viewpoint_lon(28, 8)
        {
            
        }
        
        const coordinate get_viewpoint() {
            coordinate viewpoint;
            double lat = 0.0;
            double lon = 0.0;
            read_segment(&lat, _viewpoint_lat);
            read_segment(&lon, _viewpoint_lon);
            
            viewpoint.latitude = lat;
            viewpoint.longitude = lon;
            
            return viewpoint;
        }
    private:
        segment_info _viewpoint_lat;
        segment_info _viewpoint_lon;
    };
    
    class utf8_message_send : public command_form_base_t<UTF8MESSAGESEND>
    {
    public:
        utf8_message_send(int send_node_id, int recv_node, std::string& msg)
        : command_form_base_t()
        , _sender_id_info(0, 4)
        , _recv_id_info(4, 4)
        , _msg_length_info(8, 4)
        {
            add_segment(&send_node_id, 4);
            add_segment(&recv_node, 4);
            int msg_length = msg.length();
            add_segment(&msg_length, 4);
            
            const char *tmp = msg.c_str();
            add_segment(tmp, msg_length + 1 );
            
        }
        
        utf8_message_send(const char * const input_data, const std::size_t input_data_size)
        : command_form_base_t(input_data, input_data_size)
        , _sender_id_info(0, 4)
        , _recv_id_info(4, 4)
        , _msg_length_info(8, 4)
        {
            
        }
        
        int sender_id() {
            int id = 0;
            read_segment(&id, _sender_id_info);
            
            return id;
        }
        
        int recv_id() {
            int id = 0;
            read_segment(&id, _recv_id_info);
            
            return id;
        }
        
        std::string get_msg() {
            
            int msg_length = 0;
            read_segment(&msg_length, _msg_length_info);
            
            char *msg = new char[msg_length + 1];
            
            segment_info msg_info(12, msg_length + 1);
            read_segment(msg, msg_info);
            
            msg[msg_length] = '\0';
            std::string ret_str(msg);
            
            delete [] msg;
            
            return ret_str;
        }
        
        
    private:
        segment_info _sender_id_info;
        segment_info _recv_id_info;
        segment_info _msg_length_info;
    };

}
}
