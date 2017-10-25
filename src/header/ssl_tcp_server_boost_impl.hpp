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
 @file ssl_tcp_server_boost_impl.hpp
 @author GuenYoung Lim, interruping4dev@gmail.com
 @date 2017. 8. 18.
 */

#ifndef ssl_tcp_server_boost_impl_h
#define ssl_tcp_server_boost_impl_h

#include "livemapserver_config.h"

#ifdef _DEBUG_
#include "debug_utility.hpp"
#else
#include "stdout_log_utility.hpp"
#endif

#include "boost_asio_helper.hpp"
#include "server_base.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>

#include <memory>



namespace solarcode {
namespace livemap {

    /*!
     @class
     @brief  ssl 소켓 서버 클래스
     @details 부스트 asio를 이용하여 구현한 ssl 소켓 서버a.
     @see server_base
     */
    class ssl_tcp_server_boost_impl : public server_base {
    public:
        /*!
         @breif 생성자
         */
        ssl_tcp_server_boost_impl();
        /*!
         @breif 소멸자
         */
        virtual ~ssl_tcp_server_boost_impl();
        
        /*!
         @ingroup server_base_require_impl
         @defgroup ssl_tcp_server_boost_impl_impl server_base 클래스로부터 상속되어 구현된 인터페이스 함수들.
         @{
         */
        virtual void start_service();
        virtual void stop_service();
        virtual session_builder_type get_session_builder () const;
        /*!
         @}
         *///end of ssl_tcp_server_boost_impl
    private:
        
        
        /*!
         @breif accept 작업과 관련해서 내부적으로 호출되는 함수
         @details accept 작업이 완료된 소켓 객체가 인자로 전달된다.
         @param socket 소켓 객체
         @param error boost 코드
         */
        virtual void accept_complete(std::shared_ptr<bst_ssl_tcp_socket> socket,const boost::system::error_code& error);
        
        /*!
         @breif accept 작업을 시작할 때 내부적으로 호출되는 함수
         @detail 스레드 동기화를 위해 strand 객체로 동기화가 필요하다.
         */
        virtual void _unsafe_start_service();
        
        /*!
         @breif accept 작업을 멈출 때 내부적으로 호출되는 함수
         @details 스레드 동기화를 위해 strand 객체로 동기화가 필요하다.
         */
        virtual void _unsafe_stop_service();
        
        /*!
         @breif io_service 객체
         */
        boost::asio::io_service& _ios;
        
        /*!
         @breif io_service::work
         @details io_service 객체에 작업이 POST되지 않아도 종료되지 않게한다.
         */
        boost::asio::io_service::work _work;
        
         /*!
         @breif asio acceptor
         */
        boost::asio::ip::tcp::acceptor _acceptor;
        
        /*!
         @breif 부스트 ssl 콘텍스트 객체
         */
        boost::asio::ssl::context _context;
        /*!
         @breif accept 플래그
         @details 이 플래그가 set되면 클라이언트로부터 오는 연결 수락 작업을 종료한다.
         */
        bool _is_stopped_accept;
        /*!
         @breif accept 작업에 관한 스레드 동기화를 위한 strand
         */
        boost::asio::strand _strand_for_acceptor;
        /*!
         @breif accept delegate 핸들러 처리 스레드 동기화를 위한 strand
         */
        boost::asio::strand _strand_for_acceptor_handler;
        
        /*!
         @breif io_service 스레드 그룹
         @details io_service 객체에 POST되는 작업들을 처리할 스레드 그룹.
        */
        boost::thread_group _thread_group;
    };

    
}
}

#endif /* ssl_tcp_server_boost_impl_h */
