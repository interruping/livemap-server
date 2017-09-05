/*!
 @file solarcode_command_binder.cpp
 @brief
 @detail  
 @author GeunYoung Lim, interruping@naver.com
 @date Apr 1, 2017
 @version
*/
#include "command_binder.hpp"


#include <stdint.h>
#include <dlfcn.h>
#include <chrono>
#include <fstream>
#include <cstring>

#include <openssl/sha.h>

namespace solarcode{
namespace livemap{
	void calc_file_sha1_hash(const char * const file_path, char *const hash_buffer)
	{

		std::ifstream file(file_path, std::ios::binary);
		file.seekg(0, std::ios::end);
		std::streamsize file_size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(file_size);

		file.read(buffer.data(), file_size);
		char file_sha_hash[SHA_DIGEST_LENGTH] = {0,};

		SHA1((const unsigned char *)buffer.data(), file_size, (unsigned char *)file_sha_hash);

		std::memcpy(hash_buffer, file_sha_hash, SHA_DIGEST_LENGTH);
	}

	bool compare_sha1_hash(const char * const first_sha1, const char * const second_sha1 )
	{
		bool result = std::memcmp(first_sha1, second_sha1, SHA_DIGEST_LENGTH) == 0 ?
					  true	: false;
		return result;
	}

	command_binder_type get_command_binder(void** handle)
	{

		*handle = dlopen(COMMAND_BINDER_LIB_PATH, RTLD_NOW);

		if ( *handle == nullptr ) {

		}

		command_binder_type dyload_func;

		dyload_func = (command_binder_type)dlsym(*handle, GO_BIND_COMMAND_SYMBOL);

		char * const link_error = dlerror();

		if ( link_error == nullptr ) {

		}

		return dyload_func;

	}

	std::size_t command_bind(livemap_node_database& node_db, const char * const raw_query, const std::size_t raw_query_size, const char **result_buffer)
	{
		static std::chrono::system_clock::time_point lastest_dynamic_load
			   = std::chrono::system_clock::time_point(std::chrono::seconds(0));
        
		static char lib_sha_hash[SHA_DIGEST_LENGTH] = {0,};
		static void* handle = nullptr;
		static command_binder_type do_bind_command = nullptr;

		std::chrono::system_clock::time_point time_of_now = std::chrono::system_clock::now();

		if ( lastest_dynamic_load.time_since_epoch().count() == 0 ) {
			lastest_dynamic_load = std::chrono::system_clock::now();



			calc_file_sha1_hash(COMMAND_BINDER_LIB_PATH, lib_sha_hash);
			do_bind_command = get_command_binder(&handle);
		}

		auto time_diff = time_of_now - lastest_dynamic_load;


		if ( std::chrono::duration<double>(time_diff).count() >= 10.0 ) {
			lastest_dynamic_load = std::chrono::system_clock::now();

			char tmp_hash_buffer[SHA_DIGEST_LENGTH] = {0,};

			calc_file_sha1_hash(COMMAND_BINDER_LIB_PATH, tmp_hash_buffer);

			bool is_same_hash = compare_sha1_hash(lib_sha_hash, tmp_hash_buffer);

			if( is_same_hash == false ) {
				std::memcpy(lib_sha_hash, tmp_hash_buffer, SHA_DIGEST_LENGTH);
				dlclose(handle);
				do_bind_command = get_command_binder(&handle);
			} else if ( is_same_hash == true ){

			}
		}

		return do_bind_command( node_db, raw_query, raw_query_size, result_buffer);
	}
}
}



