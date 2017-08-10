/*!
@file solarcode_tcp_server.cpp
@breif tcp 서버 관련 구현 클래스.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 09
@version 0.0.1
*/

#include "solarcode_tcp_server.hpp"
#include "solarcode_debug_utility.hpp"

#include <cstdlib>

#include <boost/bind.hpp>

namespace solarcode {
namespace livemap {
    /*!
    @breif accept 작업 후 위임 delegate 위임작업 구현 (_strand_for_acceptor 안에서 동기화 되어 실행 됨.)
    @see boost_secure_tcp_server._strand_for_acceptor   
    */
    void boost_secure_tcp_server::accept_complete(std::shared_ptr<ssl_socket_t> socket,const boost::system::error_code& error)
    {
        
        //위임 객체를 가져와 형 변환 
        boost_secure_tcp_server_delegate *delegate = (boost_secure_tcp_server_delegate*)get_delegate();
        //에러검사
        if ( error ) {
#ifdef _DEBUG_
            SC_DBGMSG("error occure while acception process.");
#endif
            //에러가 있을 시 에러 처리 위임
            delegate->error_occure(socket, error);
            _ios.post(_strand_for_acceptor.wrap(boost::bind(&boost_secure_tcp_server::_unsafe_start_accept, this)));
        } else {
#ifdef _DEBUG_
            SC_DBGMSG("acception process complete.");
#endif  
            //에러가 없으므로 소켓을 처리하도록 위임.
            delegate->handle_accept(socket);
            
            _ios.post(_strand_for_acceptor.wrap(boost::bind(&boost_secure_tcp_server::_unsafe_start_accept, this)));
        }
        
        
    }
}
}


namespace solarcode {
namespace livemap {
    /*!
    @breif 생성자 구현
    */
    boost_secure_tcp_server::boost_secure_tcp_server(boost::asio::io_service& ios, unsigned short port)
    :tcp_server_base(), //부모 초기화
    _ios(ios), //boost io_service 초기화
    _acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), //boost acceptor 최화
    _context(boost::asio::ssl::context::tlsv12), //ssl 셋팅 초기화
    _is_stopped_accept(false), //stop 플래그 초기화 default false.
    _strand_for_acceptor(ios), //스레드 동기화를 위한 strand 초기화 (for acceptor)
    _strand_for_acceptor_handler(ios) // 스레드 동기화를 위한 strand 초기화 (for accept handler)
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
        _context.use_tmp_dh_file("dh512.pem");
    }
    
    /*! 
    @breif accept 작업 시작 요청 함수 구현
    */
    void boost_secure_tcp_server::start_accept()
    {
#ifdef _DEBUG_
        SC_DBGMSG("request accept start.");
#endif
        _ios.post(_strand_for_acceptor.wrap(boost::bind(&boost_secure_tcp_server::_unsafe_start_accept, this)));
    }
    
    /*!
    @breif accept 작업 시작 함수 구현exi
    */
    void boost_secure_tcp_server::_unsafe_start_accept()
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
        std::shared_ptr<ssl_socket_t> new_socket = std::make_shared<ssl_socket_t>(_ios, _context);
        
        auto accept_complete_handler = _strand_for_acceptor_handler.wrap(boost::bind(&boost_secure_tcp_server::accept_complete, this,
                              new_socket, boost::asio::placeholders::error));
                              
        _acceptor.async_accept(new_socket->lowest_layer(), accept_complete_handler);
        // accept 작업 계속 시작.
        //start_accept();
    }
    
    /*!
    @breif accept 중단 작업 요청 함수 구현
    */
    void boost_secure_tcp_server::stop_accept()
    {
#ifdef _DEBUG_
        SC_DBGMSG("stop accept request.");
#endif
       //중단 작업 함수 동기화 실행 요청
       _ios.post(_strand_for_acceptor.wrap(boost::bind(&boost_secure_tcp_server::_unsafe_stop_accept, this)));
    }
    
    /*!
    @breif accept 중단 작업 함수 구현
    */
    void boost_secure_tcp_server::_unsafe_stop_accept()
    {
#ifdef _DEBUG_
        SC_DBGMSG("stop accept flag set.");
#endif
        //스탑 플래그 셋.
        _is_stopped_accept = true;
    }
}
}
