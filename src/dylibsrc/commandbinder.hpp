/*!
 @file solarcode_livemap_libcommandbinder.hpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Apr 1, 2017
 @version
*/



#ifndef SRC_HEADER_SOLARCODE_LIVEMAP_LIBCOMMANDBINDER_HPP_
#define SRC_HEADER_SOLARCODE_LIVEMAP_LIBCOMMANDBINDER_HPP_

#include "service_command.hpp"
#include "node_database.hpp"

extern "C"{

namespace solarcode {
namespace livemap {


	command_type read_service_type(const char * const input_buffer);
	std::size_t do_command_bind(livemap_node_database& node_db, const char *const raw_query, const std::size_t raw_query_size, const char **result_buffer);

}
}


}


#endif /* SRC_HEADER_SOLARCODE_LIVEMAP_LIBCOMMANDBINDER_HPP_ */
