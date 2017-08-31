/*!
@file solarcode_session_io_manager.cpp
@breif 세션 매니저 클래스 구현.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 03. 04
@version 0.0.1
*/
#include "session_io_manager.hpp"


namespace solarcode {
namespace livemap {
//    /*!
//    @breif 세션을 추가하여 관리하는 구현
//    @param session 신규 세션
//    */
//    void livemap_session_io_manager::start_managing_session(std::shared_ptr<boost_secure_tcp_session> session)
//    {
//        
//        session->set_delegate(this);
//
//        session->set_expire_callback(
//        [this]{
//          _session_close_callback();
//          end_managing();
//        }    
//        );
//        session->start();
//        
//    }
//    
//    /*!
//    @brief 세션의 읽기 동작 발생했을 때 호출됨.
//    @param buffer 읽은 내용을 담은 버퍼.
//    @param buffer_length 버퍼 길이.
//    */
//    void livemap_session_io_manager::readbyte(char *const buffer, const std::size_t buffer_length)
//    {
//    	_shared_query_interpreter.request_interprete(get_id(), buffer, buffer_length);
//        
//        
//    }
//    /*!
//    @breif 세션의 쓰기 동작이 발생하기 전에 호출 됨.
//    @param buffer 쓰기 동작시 쓸 내용을 위한 버퍼.
//    @param buffer_length 버퍼 길이.
//    */
//    void livemap_session_io_manager::writebyte(char *const buffer, const std::size_t buffer_length)
//    {
//        std::unique_lock<std::mutex> command_cache_lock(_command_cache_mutex);
//        _wait_command_cache_complete_fill.wait(command_cache_lock);
//    	auto command_for_write_to_socket = std::move(_command_cache.front());
//    	_command_cache.pop();
//    	command_cache_lock.unlock();
//    	command_for_write_to_socket->serialize(buffer);
//
//        
//    }
//    
//    /*!
//    @breif 세션 관리를 종료시 호출되는 콜백 함수 객체 설정 구현.
//     */
//    void livemap_session_io_manager::set_session_close_callback(std::function<void()> callback)
//    {
//        _session_close_callback = callback;
//    }
//    
//    /*!
//     @breif 세션 관리가 끝났을 때 메모리 해제를 위한 구현.
//     */
//    void livemap_session_io_manager::end_managing()
//    {
//        std::unique_ptr<livemap_session_io_manager> suicide(this);
//    }
//
//    /*!
//     @breif 쿼리 해석자 쿼리 요청 완료 후 호출되는 위임 함수.
//     */
//    void livemap_session_io_manager::complete_interprete(common_id_type user_id, const char *const query_result, std::size_t query_result_size)
//    {
//    	std::unique_lock<std::mutex> command_cache_lock(_command_cache_mutex);
//    	if ( _command_cache.empty() == false ) {
//    		command_cache_lock.unlock();
//    		_wait_command_cache_complete_fill.notify_all();
//    		return;
//    	}
//    	command_cache_lock.unlock();
//
//    	static const std::size_t query_segment_size = 512;
//
//
//   		const std::size_t new_segment_count = query_result_size % query_segment_size > 0     ?
//   											  ( query_result_size / query_segment_size ) + 1 :
//											  query_result_size / query_segment_size ;
//
//   		std::size_t index = 0;
//		const std::size_t &last_index = query_result_size;
//
//		std::queue<std::unique_ptr<command_form_base>> new_result_commands;
//
//		for ( std::size_t count = 0 ; count < new_segment_count; count++ ) {
//
//			const std::size_t actual_buffer_size = ( count == new_segment_count - 1 ) ?
//											 	 	last_index - index 				  :
//													query_segment_size;
//
//			char * const tmp_buffer = new char[actual_buffer_size];
//
//			std::memcpy( tmp_buffer, query_result + index, actual_buffer_size );
//
//			new_result_commands.push(std::make_unique<command_form_base>(tmp_buffer));
//
//			delete[] tmp_buffer;
//
//			index += query_segment_size;
//		}
//
//		command_cache_lock.lock();
//
//		for ( std::size_t count = 0; count < new_segment_count; count++ ) {
//			auto new_segment = std::move(new_result_commands.front());
//			new_result_commands.pop();
//			_command_cache.push(std::move(new_segment));
//		}
//
//		command_cache_lock.unlock();
//		_wait_command_cache_complete_fill.notify_all();
//    }

}
}
