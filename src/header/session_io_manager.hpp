/*!
@file solarcode_session_io_manager.hpp
@breif 세션 매니저 클래스 명세.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 03. 04
@version 0.0.1
*/
#include <memory>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <boost/asio.hpp>

#include "solarcode_tcp_session.hpp"
#include "solarcode_id_manager.hpp"
#include "solarcode_query_interpreter.hpp"
#include "solarcode_livemap_command_form.hpp"

namespace solarcode {
namespace livemap {
    /*!
    @class session_io_manager
    @breif 세션의 입출력 작업을 담당하는 클래스 
    @detail n/a.
    @namespace solarcode::livemap::session_io_manager
    @see tcp_session_io_delegate
    */
    template <typename session_t>
    class session_io_manager : public  tcp_session_io_delegate{
    public:
        /*!
        @breif 세션을 추가하여 입출력 관리를 시작한다.
        */
        virtual void start_managing_session(std::shared_ptr<session_t> session) = 0;
        //inherit from tcp_session_io_delegate
        virtual void readbyte(char *const buffer, const std::size_t buffer_length) = 0;
        virtual void writebyte(char *const buffer, const std::size_t buffer_length) = 0;
        
    };
    /*!
    @class livemap_session_io_manager
    @breif  boost 라이브러리로 구현된 ssl tcp 세션의 입출력 작업을 담당하는 클래스 
    @detail n/a.
    @namespace solarcode::livemap::livemap_session_io_manager
    @see livemap_session_io_manager
    */ 
    class livemap_session_io_manager : public session_io_manager<boost_secure_tcp_session>, //세션 io_manager 베이스 클래스 상
									   public common_id_holder,								// id 값 소지자 베이스 클래스 상속
									   public livemap_query_interpreter_delegate			// 쿼리 해석자 위임 매서드 인터페이스 클래스 상속
	{
    public:
        /*!
        @brief 생성
        @param io_service 부스트 asio io_service 객체
        @param shared_query_interpreter 쿼리 해석자 공유 객
        */
        livemap_session_io_manager(boost::asio::io_service& io_service, livemap_query_interpreter& shared_query_interpreter)
        :_io_service(io_service), 									//io_service 초기화
        _strand_for_session_io_manager(_io_service), 				//멀티스레드 동기화를 위한 strand 객체 초기화
		_shared_query_interpreter(shared_query_interpreter), 		//쿼리 해석자 공유 참조자 초기화
		_command_cache_mutex(),										//뮤텍스 객체 초기화
		_command_cache(),											//커맨드 캐시 큐 초기화
		_wait_command_cache_complete_fill()							//커맨드 캐시 큐 갱신 시 알림 컨디션 변수 초기화
        {

        }
        /*!
        @breif 소멸자
        */
        ~livemap_session_io_manager(){}
        //inherit from session_io_manager
        virtual void start_managing_session(std::shared_ptr<boost_secure_tcp_session> session);

        //inherit from tcp_session_io_delegate via session_io_manager  
        virtual void readbyte(char *const buffer, const std::size_t buffer_length);
        virtual void writebyte(char *const buffer, const std::size_t buffer_length);
        
        /*!
         @brief 관리하는 세션이 닫혔을 때 후 처리.
         @param 콜백 함수 객체.
         */
        virtual void set_session_close_callback(std::function<void()> callback);
        
        /*!
         @breif 세션 관리를 끝냈을 떄 자가 메모리 해체를 위해 사용하는 매서드.
         */
        virtual void end_managing();
        //inherit from livemap_query_interpreter
        virtual void complete_query_answering(common_id_type user_id, const char *const query_result, std::size_t query_result_size);

    private:
        /*!
        @breif boost asio io_service객체
        */
        boost::asio::io_service& _io_service;
        /*!
        @breif 세션 동기화를 위한 strand
        */
        boost::asio::strand _strand_for_session_io_manager;
        /*!
        @breif 세션이 만료되었을 때 호출되는 콜백.
         */
        std::function<void()> _session_close_callback;
        /*!
         @breif 쿼리 해석자 공유 객체 참조 맴버.
         */
        livemap_query_interpreter& _shared_query_interpreter;
        /*!
         @breif 쿼리 요청 후 클라이언트에게 쓰기 동작까지 캐시 동기화를 위한 뮤텍스
         */
        std::mutex _command_cache_mutex;
        /*!
         @breif 쿼리 결과 커맨드 캐시 큐, 큐에 보낼 자료가 있을 시,
         */
        std::queue< std::unique_ptr<command_form_base> > _command_cache;
        /*!
         @breif 쿼리 요청 후 쿼리 요청 응답이 도착하여 커맨드 캐시를 갱신작업을 완료했음을 알려주는 컨디션 변수.
         */
        std::condition_variable _wait_command_cache_complete_fill;
    };
    
    
}
}
