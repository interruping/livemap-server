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
 @file ssl_tcp_server_boost_impl.cpp
 @author GeunYoung Lim, interruping4dev@gmail.com
 @date 2017. 8. 27.
 */

#include "ssl_tcp_server_boost_impl.hpp"
#include "debug_utility.hpp"
#include "tcp_session.hpp"

#include <boost/bind.hpp>

namespace solarcode {
namespace livemap {
    
    //     accept 작업 후 위임 delegate 위임작업 구현 (_strand_for_accept_hander 안에서 동기화 되어 실행 됨.)
    //     _strand_for_accept_handler로 wrapping됨.
    void ssl_tcp_server_boost_impl::accept_complete(std::shared_ptr<bst_ssl_tcp_socket> socket,const boost::system::error_code& error)
    {
        
        //위임 객체를 가져와 형 변환
        server_service_delegate *delegate = (server_service_delegate*)get_delegate();
        //에러검사
        if ( error ) {
#ifdef _DEBUG_
            SC_DBGMSG("error occure while acception process.");
#endif
            //에러가 있을 시 에러 처리 위임
            _ios.post(_strand_for_acceptor.wrap(boost::bind(&ssl_tcp_server_boost_impl::_unsafe_start_service, this)));
            delegate->error_occure(socket, std::make_shared<boost::system::error_code>(error));

        } else {
#ifdef _DEBUG_
            SC_DBGMSG("acception process complete.");
#endif
            
            _ios.post(_strand_for_acceptor.wrap(boost::bind(&ssl_tcp_server_boost_impl::_unsafe_start_service, this)));
            //에러가 없으므로 소켓을 처리하도록 위임.
            delegate->handle_accept(socket);

        }
        
        
    }
}
}

namespace solarcode {
namespace livemap{
    
    //  생성자 구현
    ssl_tcp_server_boost_impl::ssl_tcp_server_boost_impl()
    //부모 초기화
    :server_base()
    //boost io_service 초기화
    , _ios(get_shared_io_service())
    , _work(_ios)
     //boost acceptor 최화
    , _acceptor(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1212))
    //ssl 셋팅 초기화
    , _context(boost::asio::ssl::context::tlsv12)
    //stop 플래그 초기화 default false.
    , _is_stopped_accept(false)
    //스레드 동기화를 위한 strand 초기화 (for acceptor)
    , _strand_for_acceptor(_ios)
    // 스레드 동기화를 위한 strand 초기화 (for accept handler)
    , _strand_for_acceptor_handler(_ios)
    // 스레드 동기화를 위한 strand 초기화 (for accept handler)
    , _thread_group()
    {
#ifdef _DEBUG_
        SC_DBGMSG("n/a.");
#endif
        
        //ssl 셋팅.
        _context.set_options(
                             boost::asio::ssl::context::default_workarounds |
                             boost::asio::ssl::context::no_sslv2 |
                             boost::asio::ssl::context::single_dh_use );
        _context.use_certificate_chain_file("server.crt");
        _context.use_private_key_file("server.key", boost::asio::ssl::context::pem);
        _context.use_tmp_dh_file("dh2048.pem");
        
        uint32_t thread_num = 4;
        
        for ( uint32_t count = 0; count < thread_num; count++ ){
            auto ios_runner = [this](){
                _ios.run();

            };
            
            _thread_group.create_thread(ios_runner);
        }
        
        
        

    }
    
    ssl_tcp_server_boost_impl::~ssl_tcp_server_boost_impl() {
        _thread_group.join_all();
    }
    
    
    //  작업 시작 요청 함수 구현
    void ssl_tcp_server_boost_impl::start_service()
    {
#ifdef _DEBUG_
        SC_DBGMSG("request accept start.");
#endif
        _ios.post(_strand_for_acceptor.wrap(boost::bind(&ssl_tcp_server_boost_impl::_unsafe_start_service, this)));
    }
    
    
    //    accept 작업 시작 함수 구현exi
    void ssl_tcp_server_boost_impl::_unsafe_start_service()
    {
#ifdef _DEBUG_
        SC_DBGMSG("accepting start.");
#endif
        //accept 중단 플래그 검사
        if ( _is_stopped_accept == true ){
#ifdef _DEBUG_
            SC_DBGMSG("Catch stop flag. acception will be stopped.");
#endif
            _is_stopped_accept = false;
            return;
        }
        //소켓 생성
        std::shared_ptr<bst_ssl_tcp_socket> new_socket = std::make_shared<bst_ssl_tcp_socket>(_ios, _context);
        
        auto accept_complete_handler
        = _strand_for_acceptor_handler.wrap(boost::bind(&ssl_tcp_server_boost_impl::accept_complete,
                                                        this,
                                                        new_socket,
                                                        boost::asio::placeholders::error));
        
        _acceptor.async_accept(new_socket->lowest_layer(), accept_complete_handler);
    }
    
    
    //  accept 중단 작업 요청 함수 구현
    
    void ssl_tcp_server_boost_impl::stop_service()
    {
#ifdef _DEBUG_
        SC_DBGMSG("stop accept request.");
#endif
        //중단 작업 함수 동기화 실행 요청
        _ios.post(_strand_for_acceptor.wrap(boost::bind(&ssl_tcp_server_boost_impl::_unsafe_stop_service, this)));
    }
    
    
    //      accept 중단 작업 함수 구현
    void ssl_tcp_server_boost_impl::_unsafe_stop_service()
    {
#ifdef _DEBUG_
        SC_DBGMSG("stop accept flag set.");
#endif
        //스탑 플래그 셋.
        _is_stopped_accept = true;
    }
    
    //  세션 빌더 리턴
    // ssl_tcp_session 클래스 객체를 생성하는 구현 반환.
    server_base::session_builder_type ssl_tcp_server_boost_impl::get_session_builder() const
    {
        session_builder_type session_builder = [](std::shared_ptr<void> socket) {
            return std::make_shared<ssl_tcp_session>(socket);
        };
        
        return session_builder;
    }
}
}
