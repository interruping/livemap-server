#include "solarcode_service_interpreter.hpp"
#include "solarcode_command_binder.hpp"

#include <stdint.h>

namespace solarcode {
namespace livemap {

    /*!
    @breif 질의를 해석해서 요청에 맞는 응답을 전달하는 구현
    */
    void livemap_query_interpreter::query_request(common_id_type user_id, const char *const raw_query, const std::size_t raw_query_size)
    {
    	char *const release_manual_buffer = new char[raw_query_size];

    	_io_service.post(
    	_strand_for_query_interpreter.wrap(
    	[&,user_id, release_manual_buffer](){
    			//쿼리작업 시작
    		const char *result_buffer = nullptr;

    		std::size_t result_size = command_bind(_node_database, raw_query, raw_query_size, &result_buffer);

    		boost::asio::strand new_strand_for_delegate(_io_service);

    		livemap_query_interpreter_delegate *delegate_will_receive_result
				= get_delegate(user_id);

    		_io_service.post(
    		new_strand_for_delegate.wrap(
    		[delegate_will_receive_result, user_id, result_size, result_buffer](){
    			delegate_will_receive_result->complete_query_answering(user_id, result_buffer,result_size );
    			delete[] result_buffer;
    		}
    		)
    		);

    		delete[] release_manual_buffer;
    	}
    	)
		);
    }
    
    /*!
    @breif 델리게이트 객체를 아이디를 기준으로 등록하는 구현
    */
    void livemap_query_interpreter::register_delegate(common_id_type delegate_id, livemap_query_interpreter_delegate *delegate)
    {
    	std::unique_lock<std::mutex> lock_map(_mutex_for_delegate_map);
        auto result = _delegates.insert({delegate_id, delegate});
    
        if( result.second ) {
            
        } else {
        //실패시
            _delegates.erase(delegate_id);
            _delegates.insert({delegate_id, delegate});
        }
        
    }
    
    /*!
    @breif 델리게이트 객체를 찾아 반환하는 구현
    */
    livemap_query_interpreter_delegate* livemap_query_interpreter::get_delegate(common_id_type delegate_id)
    {

    	std::unique_lock<std::mutex> lock_map(_mutex_for_delegate_map);
        auto result = _delegates.find(delegate_id);
        
        if ( result != _delegates.cend() ) {
            return result->second;
        }
        
        return nullptr;
    }
}
}
