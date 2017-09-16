//
//  ssl_tcp_session_boost_impl.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 26..
//
//

#ifndef ssl_tcp_session_boost_impl_h
#define ssl_tcp_session_boost_impl_h

#include <functional>

#include "session_base.hpp"
#include "boost_asio_helper.hpp"

namespace solarcode {
namespace livemap {

    
    /*!
     @class boost_secure_tcp_session
     @breif 부스트 라이브러리로 구현한 tcp 세션 객체
     @detail n/a.
     @namespace solarcode::livemap::boost_secure_tcp_session
     @see tcp_session_base
     */
    class ssl_tcp_session_boost_impl: public session_base,
    public std::enable_shared_from_this<ssl_tcp_session_boost_impl>
    {
    public:
        /*!
         @breif 생성자
         @param socket 소켓 객체
         */
        explicit ssl_tcp_session_boost_impl(std::shared_ptr<void> socket)
        :session_base(socket),//부모 초기화.
        _io_trigger_timer(get_shared_io_service()),//타머어 초기화.
        _strand_for_session(_io_trigger_timer.get_io_service()), //strand 초기화.
        _io_service(_io_trigger_timer.get_io_service()),
        _expire_callback([](){})
        {
#ifdef _DEBUG_
            SC_DBGMSG("boost secure tcp session created.");
#endif
            
        }
        
        virtual ~ssl_tcp_session_boost_impl(){
#ifdef _DEBUG_
            SC_DBGMSG("session dtor called.");
#endif
            this->stop();
            auto tmp_expire_callback = _expire_callback;
            
            auto safe_expire
            = _strand_for_session.wrap(
            [tmp_expire_callback](){
                tmp_expire_callback();
            });
            
            _io_service.post(safe_expire);
            
            
        }
        
        //override tcp_session_base class interface
        virtual void start();
        virtual void stop();
        /*!
         @breif 세션 만기 후 작업 프로시져 등록
         */
        virtual void set_expire_callback(std::function<void()> callback);
    private:
        /*!
         @breif 세션 io 수행간 간격을 두기위한 타이머
         */
        boost::asio::deadline_timer _io_trigger_timer;
        /*!
         @breif 세션 스레드 동기화를 위한 strand 객체
         */
        boost::asio::strand _strand_for_session;
        /*!
         @breif 부스트 asio 참조
         */
        boost::asio::io_service &_io_service;
        
        /*!
         @breif 세션 만료시 후작업을 위한 콜백
         */
        std::function<void()> _expire_callback;
        /*!
         @breif 스레드 세이프하지 않은 start 힘수 (strand에 wrap된 상태에서 실행된다.)
         */
        virtual void _unsafe_start();
        /*!
         @breif 스레드 세이프하지 않은 stop 함수 (strand에 wrap된 상태에서 실행된다.)
         */
        virtual void _unsafe_stop();
        /*!
         @breif 클라이언트로부터 읽는 동작을 수행한다.
         */
        virtual void io_read_trigger(const boost::system::error_code& error);
        /*!
         @breif 클라이언트로 쓰는 동작을 수행한다.
         */
        virtual void io_write_trigger(const boost::system::error_code& error);
        /*!
         @brief handshake 핸들러
         @param error 에러
         */
        virtual void handshake_complete(const boost::system::error_code &error);
        
    };
}
}

#endif /* ssl_tcp_session_boost_impl_h */
