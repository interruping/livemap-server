// Copyright 2017 GeunYoung Lim <interruping4dev@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 @file ssl_tcp_session_boost_impl.hpp
 @author GeunYoung Lim, interruping@naver.com
 @date 2017. 8. 26.
 */

#ifndef ssl_tcp_session_boost_impl_h
#define ssl_tcp_session_boost_impl_h

#include <functional>

#include "livemapserver_config.h"

#include "session_base.hpp"
#include "boost_asio_helper.hpp"

namespace solarcode {
namespace livemap {

    
    /*!
     @class ssl_tcp_session_boost_impl
     @breif ssl 소켓 세션 부스트 라이브러리 구현
     @details 부스트 asio로 구현한 ssl socket 세션 객체.
     @see session_base
     */
    class ssl_tcp_session_boost_impl: public session_base,
    public std::enable_shared_from_this<ssl_tcp_session_boost_impl>
    {
    public:
        /*!
         @breif 생성자
         @details 연결된 ssl 소켓 객체를 받아 원격 클라이언트와 입출력 작업을 한다.
         @param ssl socket 소켓 객체
         */
        explicit ssl_tcp_session_boost_impl(std::shared_ptr<void> socket)
        //부모 초기화
        :session_base(socket)
        //타머어 초기화
        , _io_trigger_timer(get_shared_io_service())
        //strand 초기화.
        , _strand_for_session(_io_trigger_timer.get_io_service())
        
        , _io_service(_io_trigger_timer.get_io_service())
        , _expire_callback([](){})
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
        /*!
         @ingroup server_base_require_impl
         @defgroup ssl_tcp_session_boost_impl_impl server_base 클래스에서 상속받아 구현한 인터페이스 함수들.
         @{
         */
        virtual void start();
        virtual void stop();
        /*!
         @}
         */
        
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
