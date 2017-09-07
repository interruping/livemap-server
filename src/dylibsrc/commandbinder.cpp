/*!
 @file solarcode_livemap_libcommandbinder.cpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Mar 31, 2017
 @version
*/

#include "commandbinder.hpp"

#include <cstring>

extern "C"{
    
#ifdef __APPLE__
    __attribute__((visibility("default")))
#endif
    std::size_t do_command_bind(solarcode::livemap::client_node_pool& node_db, char *const raw_query, const std::size_t raw_query_size, char **result_buffer){
        return solarcode::livemap::_do_command_bind(node_db, raw_query, raw_query_size, result_buffer);
    }
}
namespace solarcode {
namespace livemap {
	command_type read_service_type(const char * const input_buffer)
	{
		command_type result_service_type = DEFAULT_COMMAND_TYPE;
		std::memcpy(&result_service_type, input_buffer, sizeof(result_service_type));
		return result_service_type;
	}
    


    std::size_t _do_command_bind(client_node_pool& node_db, char *const raw_query, const std::size_t raw_query_size, char **result_buffer)
	{
		switch(read_service_type(raw_query)){
		case update_node::type : {
			break;
		}
		case command_form_base::type : {

			break;
		}


		}
	}

}
}



