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

// io_service 사용
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
// shared_ptr , weak_ptr 사용
#include <memory>

namespace solarcode{
namespace livemap {
    
    /*!
    @class tcp_server_delegate
    @breif tcp서버 delegate 매서드 가이드 추상 클래스
    @detail n/a.
    @namespace solarcode::livemap::tcp_server_delegate
    @see tcp_server
    */
    template <typename socket_t, typename error_t> 
    class tcp_server_delegate {
    public:
        /*!
        @breif 클라이언트의 연결 요청을 수락이 완료된 후 소켓 객체의 사용을 위임하는 메서드.
        @param socket 소켓 객체
        */
        
        
         
        virtual void handle_accept(std::shared_ptr<socket_t> socket) = 0;
        /*!
        @breif 클라이언트의 연결 요청을 수락하는 과정에서 에러가 발생했을 때 동작을 위임하는 매서드.
        @param socket 에러가 발생한 소켓 객체
        @param error  에러 객체
        */
        virtual void error_occure(std::shared_ptr<socket_t> socket ,error_t& error) = 0;
    };
    
    /*!
    @class tcp_server_base
    @breif tcp프로토콜 서버 베이스 클래스
    @detail n/a
    @namepace solarcode::livemap::tcp_server_base
    @see tcp_server_delegate
    */
    template < typename tcp_server_delegate_type >
    class tcp_server_base {
    public:
        /*!
        @brief 소멸자
        */
        virtual ~tcp_server_base(){};
        /*!
        @breif 클라이언 연결요청 accept 작업을 실행하는 동작을 구현할 매서드.
        @param socket 소켓 객체
        */
        virtual void start_accept() = 0;
        /*!
        @breif accept 작업을 종료하는 동작을 구현할 매서드.
        */
        virtual void stop_accept() = 0;

        /*!
        @breif delegate 객체를 설정하는 함수.
        @param delegate delegate 객체 (약한 참조)
        */
        virtual void set_delegate(tcp_server_delegate_type* delegate)
        {
            _delegate = delegate;
        }
        /*!
        @brief delegate 객체를 가져오는 함수
        @return std::weak_ptr<tcp_server_delegate> delegate 객체 (약한 참조)
        */
        virtual tcp_server_delegate_type* get_delegate() const
        {
            return _delegate;
        }
    protected:
        /*!
        @breif 생성자 (자식에서 만 호출가능)
        */
        tcp_server_base() = default;
        

    private:
        /*!
        @breif delegate 객체 멤버 포인터 
        */
        tcp_server_delegate_type *_delegate;
        
    };
}
}

namespace solarcode {
namespace livemap {
    /*!
    @breif 부스트라이브러리 ssl 소켓
    */
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_t;
    /*!
    @breif 부스트라이브러리 딜리게이트 
    */
    typedef tcp_server_delegate<ssl_socket_t, const boost::system::error_code> boost_secure_tcp_server_delegate;
    
    /*!
    @class boost_secure_tcp_server
    @brief 부스트 asio를 이용하여 구현한 ssl tcp 서버
    @detail n/a.
    @see tcp_server
    */
    class boost_secure_tcp_server : public tcp_server_base<boost_secure_tcp_server_delegate> {
    public:
        /*!
        @breif 생성자
        @param ios 부스트 io_service
        @param port 서비스 포트
        */
        boost_secure_tcp_server( boost::asio::io_service& ios, unsigned short port);
        /*!
        @breif 소멸자
        */
        virtual ~boost_secure_tcp_server(){}
        //tcp_server 메서드 오버라이딩.
        //스레드 세이프 함.
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