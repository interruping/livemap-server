/*!
@file solarcode_tcp_session.hpp
@breif tcp 연결 세션 클래스 명세.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 09
@version 0.0.1
*/

#ifndef SOLARCODE_LIVEMAP_TCP_SESSION
#define SOLARCODE_LIVEMAP_TCP_SESSION

#include "solarcode_debug_utility.hpp"
#include "solarcode_tcp_server.hpp"
#include "solarcode_query_interpreter.hpp"

#include <memory>
#include <functional>

#include <boost/date_time/posix_time/posix_time.hpp>


namespace solarcode {
namespace livemap {
    /*!
    @class tcp_session_io_delegate
    @breif 세션 io 관련 데이터 핸들링 작업을 위임하는 클래스.
    @detail n/a.
    @namespace solarcode::livemap::tcp_session_io_delegate
    @see tcp_session_base 
    */
    class tcp_session_io_delegate {
    public:
        virtual void readbyte(char *const buffer, const std::size_t buffer_length) = 0;
        virtual void writebyte(char *const buffer, const std::size_t buffer_length) = 0;
    };
    /*!
    @class tcp_session_base
    @brief 연결된 클라이언트의 소켓을 관리하는 베이스 클래스
    @detail n/a.
    @namespace solarcode::livemap::tcp_session_base
    @see
    */
    template <typename socket_t>
    class tcp_session_base {
    public:
        /*!
        @breif 기본 생성자는 사용하지 않는다.
        */
        tcp_session_base() = delete;
        /*!
        @breif 생성자
        @param socket 레퍼런스 카운터 방식의 소켓 포인터
        */
        explicit tcp_session_base(std::shared_ptr<socket_t> socket)
        :_socket(socket), //소켓 객체 초기화.ㄴ
        _delegate(nullptr) //델리게이트 객체 초기화.
        {
#ifdef _DEBUG_
             SC_DBGMSG("session create.");
#endif
        }
        /*!
        @breif 소멸자 내부에서 세션 작동을 멈추는 매서드를 호출한다.
        */
        virtual ~tcp_session_base() {
#ifdef _DEBUG_
            SC_DBGMSG("session closed.");
#endif
            
        }
        /*!
        @breif 세션을 시작한다.
        */
        virtual void start() = 0;
        /*!
        @breif 세션을 끝낸다.
        */
        virtual void stop() = 0;
        /*!
        @breif 델리게이트를 설정하는 setter.
        @param delegate 세션 io 위임. 
        */
        virtual void set_delegate(tcp_session_io_delegate *delegate) 
        {
#ifdef _DEBUG_
            SC_DBGMSG("tcp_session_io_delegate set.");
#endif
            _delegate = delegate;
        }
        /*!
        @breif 델리게이트를 설정하는 getter.
        @return tcp_session_io_delegate 세션 io 위임.
        */
        virtual tcp_session_io_delegate* get_delegate() const 
        {
#ifdef _DEBUG_
            SC_DBGMSG("tcp_session_io_delegate get.");
#endif
            return _delegate;
        }
    protected:
        /*!
        @breif 서브클래스에서 소켓 객체 접근을 위한 개터
        */
        virtual std::shared_ptr<socket_t> get_socket() const 
        {
#ifdef _DEBUG_
            SC_DBGMSG("socket object returned.");
#endif
            return _socket;
        }
    private:
        /*!
        @breif 소켓 객체
        */
        std::shared_ptr<socket_t> _socket;
        /*!
        @breif 델리게이트 포인터
        */
        tcp_session_io_delegate *_delegate;

    };
    
    /*!
    @class boost_secure_tcp_session
    @breif 부스트 라이브러리로 구현한 tcp 세션 객체
    @detail n/a.
    @namespace solarcode::livemap::boost_secure_tcp_session
    @see tcp_session_base
    */
    class boost_secure_tcp_session: public tcp_session_base<ssl_socket_t>,
                                    public std::enable_shared_from_this<boost_secure_tcp_session>
    {
    public:
        /*!
        @breif 생성자
        @param socket 소켓 객체
        */
        explicit boost_secure_tcp_session(std::shared_ptr<ssl_socket_t> socket)
        :tcp_session_base<ssl_socket_t>(socket),//부모 초기화.
        _io_trigger_timer( (socket->lowest_layer()).get_io_service()),//타머어 초기화.
        _strand_for_session( _io_trigger_timer.get_io_service()), //strand 초기화.
        _io_service(_io_trigger_timer.get_io_service()),
        _expire_callback(nullptr)
        {
#ifdef _DEBUG_
            SC_DBGMSG("boost secure tcp session created.");
#endif 
            
        }
        virtual ~boost_secure_tcp_session(){
#ifdef _DEBUG_
            SC_DBGMSG("session dtor called.");
#endif 
            this->stop();
            auto tmp_expire_callback = _expire_callback;
            _io_service.post(
            _strand_for_session.wrap(
            [tmp_expire_callback](){
                tmp_expire_callback();
            }    
            )
            );
            
        
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

#endif
