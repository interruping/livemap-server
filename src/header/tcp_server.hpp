/*!
@file solarcode_tcp_server.hpp
@breif tcp 서버 관련 클래스.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 05
@version 0.0.1
*/
#ifndef SOLARCODE_LIVEMAP_TCP_SERVER
#define SOLARCODE_LIVEMAP_TCP_SERVER

#include "server_base.hpp"

// io_service 사용
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
// shared_ptr , weak_ptr 사용
#include <memory>

namespace solarcode {
namespace livemap {
    /*!
    @breif 부스트라이브러리 ssl 소켓
    */
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_t;
//    /*!
//    @breif 부스트라이브러리 딜리게이트 
//    */
//    typedef tcp_server_delegate<ssl_socket_t, const boost::system::error_code> boost_secure_tcp_server_delegate;
//    
    /*!
    @class ssl_tcp_server
    @brief 부스트 asio를 이용하여 구현한 ssl tcp 서버
    @detail n/a.
    @see tcp_server
    */
    class ssl_tcp_server : public server_base {
    public:
        /*!
        @breif 생성자
        @param ios 부스트 io_service
        @param port 서비스 포트
        */
        ssl_tcp_server( boost::asio::io_service& ios, unsigned short port);
        /*!
        @breif 소멸자
        */
        virtual ~ssl_tcp_server(){}
        
    
        //base_server 메서드 오버라이딩.
        //스레드 세이프 함.
        virtual void *get_new_instance();
        virtual void start_accept();
        virtual void stop_accept();

    private:
        
        
        /*!
        @breif accept 작업과 관련해서 내부적으로 호출되는 함수
        @param socket 소켓 객체
        @param error 에러 객체go
        */
        virtual void accept_complete(std::shared_ptr<ssl_socket_t> socket,const boost::system::error_code& error);
        /*!
        @breif accept 작업을 시작할 때 내부적으로 호출되는 함수
        @detail 스레드 동기화를 위해 strand 객체로 동기화 시켜야 함.
        */
        virtual void _unsafe_start_accept();
        /*!
        @breif accept 작업을 멈출 때 내부적으로 호출되는 함수
        @detail 스레드 동기화를 위해 strand 객체로 동기화 시켜야 함.
        */
        virtual void _unsafe_stop_accept();
        /*! 
        @breif io_service 객체
        */
        boost::asio::io_service& _ios;
        /*!
        @breif 부스트 asio acceptor
        */
        boost::asio::ip::tcp::acceptor _acceptor;
        /*!
        @breif 부스트 ssl 콘텍스트 객체
        */
        boost::asio::ssl::context _context;
        /*!
        @breif accept 플래그
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
    };
}
}


#endif //pair of #ifndef SOLARCODE_LIVEMAP_TCP_SERVER
