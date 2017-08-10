/*!
@file solarcode_command_binder.hpp
@breif 버퍼를 분석해 명령으로 변환해주는
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 03. 09.
@version 0.0.1
*/

#ifndef SRC_HEADER_SOLARCODE_COMMAND_BINDER_HPP_
#define SRC_HEADER_SOLARCODE_COMMAND_BINDER_HPP_

#include "solarcode_livemap_node_database.hpp"

#include <vector>

namespace solarcode {
namespace livemap {

	static const char * const COMMAND_BINDER_LIB_PATH = "./lib/libcommandbinder.so";
	static const char * const GO_BIND_COMMAND_SYMBOL = "do_bind_command";

	using command_binder_type = std::size_t(*)(livemap_node_database&, const char *const, const std::size_t, const char**);

	void calc_file_sha1_hash(const char * const file_path, char *const hash_buffer);


	bool compare_sha1_hash(const char * const first_sha1, const char * const second_sha1 );

	command_binder_type get_command_binder(void** handle);

	std::size_t command_bind(livemap_node_database& node_db, const char * const raw_query, const std::size_t raw_query_size, const char **result_buffer);
}
}




#endif /* SRC_HEADER_SOLARCODE_COMMAND_BINDER_HPP_ */
