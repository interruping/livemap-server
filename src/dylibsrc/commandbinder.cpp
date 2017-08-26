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

namespace solarcode {
namespace livemap {
	command_type read_service_type(const char * const input_buffer)
	{
		command_type result_service_type = DEFAULT_COMMAND_TYPE;
		std::memcpy(&result_service_type, input_buffer, sizeof(result_service_type));
		return result_service_type;
	}

	std::size_t do_command_bind(livemap_node_database& node_db, const char *const raw_query, const std::size_t raw_query_size, const char **result_buffer)
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


}
