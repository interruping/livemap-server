/*!
 @file session_base.hpp
 @author GeunYoung Lim, interruping4dev@gmail.com
 @date 2017. 08. 18.
 */
#ifndef session_base_h
#define session_base_h

#include "debug_utility.hpp"

#include <memory>
#include <functional>

#include <boost/date_time/posix_time/posix_time.hpp>


namespace solarcode {
namespace livemap {
    /*!
     @breif session_io_delegate 세션 io 처리 인터페이스 클래스.
     @detail session_base 계열 클래스가 참조하여 사용하는 클래스.
             세션 클래스에서 클라이언트로부터 보내고 받는 데이터를 처리하는 인터페이스 클래스.
             session_base 계열 클래스가 이 클래스 객체의 수명을 관리한다.
     @see session_base
     */
    class session_io_delegate {
    public:
        /*!
         @breif 가상 소멸자
         */
        virtual ~session_io_delegate() {};
        
        /*!
         @defgroup session_io_delegate_require_impl session_io_delegate클래스의 서브 클래스에서 구현해야하는 인터페이스
         @{
         */
        
        /*!
         @breif 세션 객체가 클라이언트로부터 받은 데이터를 넘겨주는 함수.
         @details 클라이언트가 보낸 데이터 처리 구현이 이 함수에 구현하면 된다.
                  클라이언트로부터 온 데이터는 buffer에 담겨오는데 이 함수 사용자는 buffer의 메모리 해제 책임이 없다.
                  buffer의 메모리는 세션 객체가 관리한다.
         @param buffer 사용자로부터 읽은 raw data.
         @param buffer_length 사용자로부터 읽은 raw data 길이 (Byte 단위)
         */
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length) = 0;
        
        /*!
         @breif 세션 객체가 클라이언트로 데이터를 보내기 전 보낼 데이터를 결정하는 함수.
         @details 클라이언트로 보내고 싶은 데이터를 결정하여 buffer 변수에 쓰면 된다.
                  buffer에 쓸 데이터만큼 정확하게 메모리를 직접할당하여야한다.
                  buffer에 쓴 데이터 길이를 바이트 단위로 리턴해줘야 세션객체가 데이터를 보낼 수 있다.
                  buffer에 쓴 길이와 리턴한 길이가 다를경우 그 세션은 만료된다.
                  buffer에 할당한 메모리는 세션객체가 해제하므로 임의로 해제해선 안된다.
         
         @code{.cpp}
         *buffer = new char[77]; // 할당한 버퍼 데이터 사이즈와
         return std::size_t(77); // 리턴하는 사이즈가 같아야 함 (바이트 단위).
         @endcode
         
         */
        virtual std::size_t session_write_before_buffer(char ** buffer) = 0;
        /*
         @}
         */ //end of session_io_delegate_require_impl
        
    
    };
    
    /*!
     @brief session_base 연결된 클라이언트의 소켓 입출력을 관리하는 세션 베이스 클래스.
     @detail session_base 연결된 클라이언트 소켓의 입출력을 하는 클래스며 입출력할 데이터 처리는 session_io_delegate 계열
             클래스에 위임한다. 이 클래스를 상속하는 클래스는 소켓 입출력 에러가 발생하면 세션 객체는 세션을 만료하도록 구현되어야한다.
     
     @see session_io_delegate
     */
    class session_base {
    public:
        /*!
         @breif 기본 생성자는 사용하지 않는다.
         */
        session_base() = delete;
        /*!
         @breif 소켓 객체 포인터를 인자로 받는 생성자.
         @detail 소켓 객체 포인터는 std::shared_ptr로 wrapping된 포인터이다.
         @param socket 소켓 객체 포인터
         */
        explicit session_base(std::shared_ptr<void> socket)
        :_socket(socket)
        , _delegate(nullptr)
        , _expire_callback(nullptr)
        {
#ifdef _DEBUG_
            SC_DBGMSG("session create.");
#endif
        }
        
        /*!
         @breif 가상 소멸자
         @detail 소멸자를 안에서 _expire_callback 멤버 함수 객체를 실행한다.
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
         @defgroup session_base_require_impl session_base 클래스의 서브 클래스가 구현해야하는 함수들.
         @{
         */
        
        /*!
         @breif 세션을 시작하는 인터페이스.
         @detail 이 클래스를 상속하는 클래스는 세션이 가지고 있는 소켓에 입출력을 시작도록
                 구현되어야 한다.
         */
        virtual void start() = 0;
        /*!
         @breif 세션을 끝내는 인터페이스.
         @detail 이 클래스를 상속하는 클래스는 세션이이 가지고 있는 소켓을 입출력을 중단하도록 구현해야 한다.
                 또한 이 함수가 호출된 직후 메모리에서 해제하여 세션을 만료하도록 구현해야 한다.
         */
        virtual void stop() = 0;
        
        /*!
         @}
         */ //end of session_base_require_impl
        
        
        /*!
         @breif session_io_delegate 계열 클래스 객체를 셋팅하는 setter.
         @detail session_io_delegate 계열 클래스 객체를 받아 소켓 입출력에 필요한 데이터를 얻는다.
                 session_io_delegate 계열 클래스 객체의 수명은 이 클래스 객체가 관리한다.
                 셋팅된 session_io_delegate 계열 클래스 객체는 이 클래스의 unique_ptr로 wrapping되어 관리된다.
                 따라서 이 클래스의 객체가 소멸될 떄 session_io_delegate 객체도 소멸된다.
         @param delegate session_io_delegate 계열 클래스 객체 포인터.
         */
        virtual void set_delegate(session_io_delegate *delegate)
        {
#ifdef _DEBUG_
            //SC_DBGMSG("session_io_delegate set.");
#endif
            _delegate.reset(delegate);
        }
        
        /*!
         @breif session_io_delegate 계열 클래스 객체 getter.
         @detail 이 클래스 객체가 참조하고 잇는 session_io_delegate 객체의 포인터를 반환한다.
                 이 함수를 호출하여도 이 클래스 객체는 계속해서 참조한다. 따라서 반환받은 포인터 객체는
                 이 클래스 객체에 의해 임의로 메모리에서 해제된다.
                 이 함수는 이 클래스와 이 클래스를 상속받는 클래스 안에서만 사용하는 것이 안전하다.
         @return session_io_delegate 계열 클래스 객체 포인터
         */
        virtual session_io_delegate* get_delegate() const
        {
#ifdef _DEBUG_
            //SC_DBGMSG("session_io_delegate get.");
#endif
            return _delegate.get();
        }
        
        /*!
         @breif 서브클래스에서 소켓 객체 접근을 위한 getter.
         @detail 소켓 객체 포인터를 shared_ptr로 wrapping하여 반환한다.
         */
        virtual std::shared_ptr<void> get_socket() const
        {
#ifdef _DEBUG_
            //SC_DBGMSG("socket object returned.");
#endif
            return _socket;
        }

        /*!
         @breif 이 세션 객체가 만료되기 직전에 실행할 작업을 설정하는 setter.
         @detail 세션 객체가 소멸될 때 _expire_callback 멤버 함수 객체를 실행한다.
         @see _expire_callback
         */
        virtual void set_expire_callback(std::function<void()> expire_callback) {
#ifdef _DEBUG_
            SC_DBGMSG("set expire callback");
#endif
            _expire_callback = expire_callback;
        }
    private:
         /*!
          @breif 소켓 객체 포인터
          @detail shared_ptr로 wrapping되었다.
         */
        std::shared_ptr<void> _socket;
        /*!
         @breif session_io_delegate 계열 클래스의 객체 포인터
         @detail unique_ptr로 warpping되었다.따라서 session_base 객체가 소멸되면 같이 소멸된다.
         */
        std::unique_ptr<session_io_delegate> _delegate;
        /*!
         @breif 세션 만료 콜백 함수 객체
         @detail 소멸자에서 실행된다.
         */
        std::function<void()> _expire_callback;
        
    };
    
}
}

#endif /* session_base_h */
