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
        
		switch(read_service_type(raw_request)){
            case request_user_info::type : {
                
                common_id_type id = request_node_s->get_id();
                std::cout << "id is " << id << std::endl;
#ifdef _DEBUG_
                SC_DBGMSG("server serve new id to client id is :" + id);
#endif
                set_user_info set_user_info_reply(id);
                
                *result_buffer = new char[set_user_info_reply.get_entire_size() + sizeof(command_type)];
                
                command_type type = set_user_info::type;
                std::memcpy(*result_buffer, &type, sizeof(type));
                
                set_user_info_reply.serialize(*result_buffer + sizeof(type));
                
                return set_user_info_reply.get_entire_size() + sizeof(command_type);
                
                break;
            }
            case user_update_node::type : {
            
                request_node_s->set_coordinate(make_coordinate(0.0f, 0.0f));
                
                
                break;
            }
            case command_form_base::type : {

                break;
            }


		}
	}

}
}



