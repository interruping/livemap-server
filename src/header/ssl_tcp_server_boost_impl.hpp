//
//  ssl_tcp_server_boost_impl.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 18..
//
//

#ifndef ssl_tcp_server_boost_impl_h
#define ssl_tcp_server_boost_impl_h

#include "boost_asio_helper.hpp"
#include "server_base.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <memory>

namespace solarcode {
namespace livemap {

    /*!
     @class ssl_tcp_server
     @brief 부스트 asio를 이용하여 구현한 ssl tcp 서버
     @detail n/a.
     @see tcp_server
     */
    class ssl_tcp_server_boost_impl : public server_base {
    public:
        /*!
         @breif 생성자
         @param ios 부스트 io_service
         @param port 서비스 포트
         */
        ssl_tcp_server_boost_impl();
        /*!
         @breif 소멸자
         */
        virtual ~ssl_tcp_server_boost_impl(){}
        
        
        //base_server 메서드 오버라이딩.
        //스레드 세이프 함.
        virtual void start_service();
        virtual void stop_service();
        virtual session_builder_type get_session_builder () const;
        
    private:
        
        
        /*!
         @breif accept 작업과 관련해서 내부적으로 호출되는 함수
         @param socket 소켓 객체
         @param error 에러 객체go
         */
        virtual void accept_complete(std::shared_ptr<bst_ssl_tcp_socket> socket,const boost::system::error_code& error);
        /*!
         @breif accept 작업을 시작할 때 내부적으로 호출되는 함수
         @detail 스레드 동기화를 위해 strand 객체로 동기화 시켜야 함.
         */
        virtual void _unsafe_start_service();
        /*!
         @breif accept 작업을 멈출 때 내부적으로 호출되는 함수
         @detail 스레드 동기화를 위해 strand 객체로 동기화 시켜야 함.
         */
        virtual void _unsafe_stop_service();
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

#endif /* ssl_tcp_server_boost_impl_h */
