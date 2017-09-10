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


extern "C"{
    std::size_t do_command_bind(std::weak_ptr<solarcode::livemap::client_node> request_node ,
                                solarcode::livemap::client_node_pool& node_db ,
                                char *const raw_request ,
                                const std::size_t raw_request_size ,
                                char **result_buffer );
}
namespace solarcode {
namespace livemap {

    
	command_type read_service_type(const char * const input_buffer);
    
    std::size_t _do_command_bind(std::weak_ptr<client_node> request_node,
                                 client_node_pool& node_db,
                                 char *const raw_request,
                                 const std::size_t raw_request_size,
                                 char **result_buffer);

}
}



#endif /* SRC_HEADER_SOLARCODE_LIVEMAP_LIBCOMMANDBINDER_HPP_ */
