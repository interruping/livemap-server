/*!
@file solarcode_query_interpreter.hpp
@breif 쿼리문 해석 클래스 명세.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 03. 04
@version 0.0.1
*/
#ifndef SOLARCODE_LIVEMAP_QUERY_INTERPRETER
#define SOLARCODE_LIVEMAP_QUERY_INTERPRETER

#include "solarcode_id_manager.hpp"
#include "solarcode_livemap_node_database.hpp"

#include <mutex>

#include <boost/asio.hpp>

namespace solarcode {
namespace livemap {
    /*!
    @class query_interpreter_delegate
    @breif 쿼리 해석 클래스 델리게이트 클래스.
    @detail n/a.
    @namespace solarcode::livemap::query_interpreter_delegate
    @see 
    */
    template <typename user_id_type>
    class query_interpreter_delegate {
    public:
    virtual void complete_query_answering(user_id_type user_id, const char *const query_result, std::size_t query_result_size) = 0;
    };
    
    /*!
    @class query_interpreter
    @breif 쿼리 해석 클래스.
    @detail n/a.
    @namespace solarcode::livemap::query_interpreter
    @see 
    */
    template <typename user_id_type>
    class query_interpreter {
    public:
    virtual void query_request(user_id_type user_id, const char * const raw_query, const std::size_t raw_query_size) = 0;
    };
}
}

namespace solarcode {
namespace livemap {
    /*!
    @breif 라이브맵 쿼리 해석자 베이스 타입
    */
    using livemap_query_interpreter_base = query_interpreter<common_id_type>;
    
    /*!
    @breif 라이브맵 쿼리 해석자 델리게이트 타입
    */
    using livemap_query_interpreter_delegate = query_interpreter_delegate<common_id_type>;

    /*!
    @class livemap_query_interpreter
    @breif 라이브맵 쿼리 해석 클래스.
    @detail n/a.
    @namespace solarcode::livemap::livemap_query_interpreter
    @see 
    */
    class livemap_query_interpreter : public livemap_query_interpreter_base {
    public:
        /*!
        @breif 생성자
        @param io_service 부스트 asio io_service 객체
        */
        livemap_query_interpreter(boost::asio::io_service& io_service)
        :_io_service(io_service),
        _strand_for_query_interpreter(io_service),
		_delegates(),
		_node_database(),
		_mutex_for_delegate_map()
        {
            
        }
       virtual void query_request(common_id_type user_id, const char *const raw_query, const std::size_t raw_query_size);
        /*!
        @breif 델리게이트 등록 매서드
        @param delegate
        @param delegate 델리게이트 객체
        */
       virtual void register_delegate(common_id_type delegate_id,livemap_query_interpreter_delegate *delegate);
        /*!
        @breif 델리게이트 객체 게터 매서드
        */
       virtual livemap_query_interpreter_delegate* get_delegate(common_id_type delegate_id);
    private:
        /*!
        @breif 부스트 io_service 객체
        */
        boost::asio::io_service& _io_service;
        /*!
        @breif 동기화를 위한 strand 객체
        */
        boost::asio::strand _strand_for_query_interpreter;
        /*!
        @breif 델리게이트 객체 멤버 포인터
        */
        std::map<common_id_type, livemap_query_interpreter_delegate *> _delegates;
        /*!
        @breif 라이브맵 노드 데이터 베이스 객체
         */
        livemap_node_database _node_database;
        /*!
         @breif 델리게이트 객체 컨테이너 접근 동기화 뮤텍스
         */
        std::mutex _mutex_for_delegate_map;
    };
}
}
#endif
