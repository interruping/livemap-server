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
 @file solarcode_livemap_libcommandbinder.cpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Mar 31, 2017
 @version
*/

#include "commandbinder.hpp"
#include "debug_utility.hpp"
#include "geo_calc_distance.h"
#include <cstring>
#include <iostream>

extern "C"{
    
#ifdef __APPLE__
    __attribute__((visibility("default")))
#endif
    std::size_t do_command_bind(std::weak_ptr<solarcode::livemap::client_node> request_node ,
                                solarcode::livemap::client_node_pool& node_db ,
                                char *const raw_request ,
                                const std::size_t raw_request_size ,
                                char **result_buffer )
    {
        return solarcode::livemap::_do_command_bind(request_node, node_db, raw_request, raw_request_size, result_buffer);
    }
}
namespace solarcode {
namespace livemap {
	command_type read_service_type(const char * const input_data)
	{
		command_type result_service_type = DEFAULT_COMMAND_TYPE;
		std::memcpy(&result_service_type, input_data, sizeof(result_service_type));
		return result_service_type;
	}
    
    

    std::size_t _do_command_bind(std::weak_ptr<client_node> request_node,
                                 client_node_pool& node_db,
                                 char *const raw_request,
                                 const std::size_t raw_request_size,
                                 char **result_buffer)
	{
        std::shared_ptr<client_node> request_node_s = request_node.lock();
        
        if ( request_node_s == nullptr ) {
            return 0;
        }
        
        printf("service type received -- %d\n", read_service_type(raw_request));
		switch(read_service_type(raw_request)){
            case request_user_info::type : {
                
                common_id_type id = request_node_s->get_id();

                set_user_info set_user_info_reply(id);
                
                *result_buffer = new char[set_user_info_reply.get_entire_size() + sizeof(command_type)];
                
                command_type type = set_user_info::type;
                std::memcpy(*result_buffer, &type, sizeof(type));
                
                set_user_info_reply.serialize(*result_buffer + sizeof(type));
                
                return set_user_info_reply.get_entire_size() + sizeof(command_type);
                
                break;
            }
            case user_viewpoint_update::type :
            case user_update_node::type : {
                
                
                if ( node_db.msg_check(request_node_s->get_id()) ) {
                    auto sender_id_and_msg = node_db.get_msg(request_node_s->get_id());
                    utf8_message_send to_send_msg(sender_id_and_msg.first, request_node_s->get_id(), sender_id_and_msg.second);
                    std::cout << "message_sended to " << request_node_s->get_id() << std::endl;
                    int type = utf8_message_send::type;
                    *result_buffer = new char[to_send_msg.get_entire_size() + sizeof(command_type)];
                    std::memcpy(*result_buffer, &type, sizeof(command_type));
                    
                    to_send_msg.serialize(*result_buffer + sizeof(command_type));
                    
                    return to_send_msg.get_entire_size() + sizeof(command_type);
                    
                }
            
                user_update_node recieve_command(raw_request + 4, raw_request_size);
                client_node update_info = recieve_command.get_client_node_for_update();
                request_node_s->set_coordinate(
                update_info.getCoordinate()
                );
                
                
                std::vector<client_node> near_nodes;
                
                coordinate request_node_coord;
                
                int type4coordbind = read_service_type(raw_request);
                
                if ( type4coordbind == user_update_node::type ){
                    request_node_coord = request_node_s->getCoordinate();
                } else if ( type4coordbind == user_viewpoint_update::type ) {
                    user_viewpoint_update vp_update(raw_request + 4, raw_request_size);
                    request_node_coord = vp_update.get_viewpoint();
                }
                
                node_db.scan_all_nodes([&](std::weak_ptr<const client_node> other_node){
                    auto other_node_s = other_node.lock();
                    
                    if ( request_node_s->get_id() == other_node_s->get_id() ) {
                        return;
                    }
                    
                    
                    coordinate other_node_coord = other_node_s->getCoordinate();
                    
                    double calc_distance = distanceEarth(request_node_coord.latitude,
                                                    request_node_coord.longitude,
                                                    other_node_coord.latitude,
                                                    other_node_coord.longitude);
                    
                    if ( calc_distance <= LMS_CFG_DETECTABLE_DISTANCE_KILOMETER ) {
                        client_node new_near_node(*other_node_s);
                        
                        near_nodes.push_back(new_near_node);
                    }
                    
                    
                    
                });
                
                near_node_info near_node_info_command;
                near_node_info_command.set_near_node(near_nodes);
                
                int type = near_node_info::type;
                *result_buffer = new char[near_node_info_command.get_entire_size() + sizeof(type)];
                int buffer_size = near_node_info_command.get_entire_size() + sizeof(type);
                std::memcpy(*result_buffer, &type, sizeof(type));
                
                near_node_info_command.serialize(*result_buffer + sizeof(type));
               
                return near_node_info_command.get_entire_size() + sizeof(type);
                
                break;
            }
            case utf8_message_send::type : {
                utf8_message_send receive_msg(raw_request + 4, raw_request_size - 4);
                node_db.save_msg(receive_msg.sender_id(), receive_msg.recv_id(), receive_msg.get_msg());
                std::cout << "message_recieved from " << receive_msg.sender_id() << " to " << receive_msg.recv_id() << " msg : " << receive_msg.get_msg() << std::endl;
                
                
                *result_buffer = new char[4];
                int type =command_form_base::type;
                
                std::memcpy(*result_buffer, &type, 4);
                
                return 4;
            }
            case command_form_base::type : {
                
                if ( node_db.msg_check(request_node_s->get_id()) ) {
                    auto sender_id_and_msg = node_db.get_msg(request_node_s->get_id());
                    utf8_message_send to_send_msg(sender_id_and_msg.first, request_node_s->get_id(), sender_id_and_msg.second);
                    
                    std::cout << "message_sended to " << request_node_s->get_id() << std::endl;
                    
                    int type = utf8_message_send::type;
                    *result_buffer = new char[to_send_msg.get_entire_size() + sizeof(command_type)];
                    std::memcpy(*result_buffer, &type, sizeof(command_type));
                    
                    to_send_msg.serialize(*result_buffer + sizeof(command_type));
                    
                    return to_send_msg.get_entire_size() + sizeof(command_type);
                    
                }
                
                
                *result_buffer = new char[4];
                int type =command_form_base::type;
                
                std::memcpy(*result_buffer, &type, 4);
            
                return 4;
                break;
            }

                
		}
        
        return 0;
	}

}
}



