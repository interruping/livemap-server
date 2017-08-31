//
//  session_base.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 18..
//
//

#ifndef session_base_h
#define session_base_h

#include "debug_utility.hpp"
//#include "tcp_server.hpp"
//#include "service_interpreter.hpp"

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
        class session_io_delegate {
        public:
            virtual ~session_io_delegate() {};
            virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length) = 0;
            virtual void session_write_before_buffer(char *const buffer, const std::size_t buffer_length) = 0;
        };
        /*!
         @class tcp_session_base
         @brief 연결된 클라이언트의 소켓을 관리하는 베이스 클래스
         @detail n/a.
         @namespace solarcode::livemap::tcp_session_base
         @see
         */
        class session_base {
        public:
            /*!
             @breif 기본 생성자는 사용하지 않는다.
             */
            session_base() = delete;
            /*!
             @breif 생성자
             @param socket 레퍼런스 카운터 방식의 소켓 포인터
             */
            explicit session_base(std::shared_ptr<void> socket)
            :_socket(socket), //소켓 객체 초기화.
            _delegate(nullptr), //델리게이트 객체 초기화.
            _expire_callback(nullptr)
            {
#ifdef _DEBUG_
                SC_DBGMSG("session create.");
#endif
            }
            /*!
             @breif 소멸자 내부에서 세션 작동을 멈추는 매서드를 호출한다.
             */
            virtual ~session_base() {
#ifdef _DEBUG_
                SC_DBGMSG("session closed.");
#endif
                if (_expire_callback != nullptr ) {
                    _expire_callback();
                }
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
            virtual void set_delegate(session_io_delegate *delegate)
            {
#ifdef _DEBUG_
                SC_DBGMSG("session_io_delegate set.");
#endif
                _delegate.reset(delegate);
            }
            /*!
             @breif 델리게이트를 설정하는 getter.
             @return tcp_session_io_delegate 세션 io 위임.
             */
            virtual session_io_delegate* get_delegate() const
            {
#ifdef _DEBUG_
                SC_DBGMSG("session_io_delegate get.");
#endif
                return _delegate.get();
            }
            
            /*!
             @breif 서브클래스에서 소켓 객체 접근을 위한 개터
             */
            virtual std::shared_ptr<void> get_socket() const
            {
#ifdef _DEBUG_
                SC_DBGMSG("socket object returned.");
#endif
                return _socket;
            }

            /*!
             @breif 세션 만료 람다 세터
             */
            virtual void set_expire_callback(std::function<void()> expire_callback) {
#ifdef _DEBUG_
                SC_DBGMSG("set expire callback");
#endif
                _expire_callback = expire_callback;
            }
        private:
             /*!
             @breif 소켓 객체
             */
            std::shared_ptr<void> _socket;
            /*!
             @breif 델리게이트 포인터
             */
            std::unique_ptr<session_io_delegate> _delegate;
            /*!
             @breif 세션 만료 콜백
             */
            std::function<void()> _expire_callback;
            
        };
        
    }
}

#endif /* session_base_h */
