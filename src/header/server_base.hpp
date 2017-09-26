/*!
 @file server_base.hpp
 @author GeunYoung Lim, interruping4dev@gmail.com
 @date 2017. 08. 15
 */

#ifndef server_base_h
#define server_base_h

#include <memory>
#include <functional>

#include "session_base.hpp"

namespace solarcode {
namespace livemap {
    
    /*!
     @brief server_base 클래스 전방 선언.
     server_service_deleget 추상 클래스의 set_target_server(server_base *) 인터페이스에 사용.
     */
    class server_base;
    
    /*!
     @breif 서버 서비스 위임 추상 클래스.
     @detail server_base 클래스에서 연결된 소켓을 받아 서비스를 제공하는 인터페이스 제공.
             서비스 제공 중 발생하는 에러처리 인터페이스도 제공.
             이 클래스를 상속하는 클래스의 수명은 server_base 클래스에의해 결정된다.
     @see server_base
     */
    class server_service_delegate  {
    public:
        /*!
         @breif 가상 소멸자
         */
        virtual ~server_service_delegate() {}
        /*!
         @breif server_base 클래스를 약한 참조하기 위한 클래스
         @detail server_base 클래스의 set_delegate 함수 안에서 호출된다.
                 server_base 클래스의 session_builder 핸들을 받기 위해 사용된다.
         @param target_server server_service_delegate를 참조하고 있는 target_server
         */
        virtual void set_target_server(server_base *target_server) = 0;
        /*!
         @breif 클라이언트의 연결 요청을 수락이 완료된 후 소켓 객체의 사용을 위임하는 메서드.
         @detail 클라이언트에서 넘겨받은 소켓 타입은 void 포인터다. 소켓타입은 server_service_delegate
                 가 몰라도 server_base에서 넘겨받은 session_builder로 세션을 생성할 수 있다.
         @param socket 소켓 객체
         */
        virtual void handle_accept(std::shared_ptr<void> socket) = 0;
        /*!
         @breif 클라이언트의 연결 요청을 수락하는 과정에서 에러가 발생했을 때 동작을 위임하는 매서드.
         @detail 에러가 발생했을때 오류가 발생한 소켓과 에러 정보가 같이 전달된다. 소켓객체와 에러객체는 이 메서드에서 수명이 끝나야 한다.
         @param socket 에러가 발생한 소켓 객체
         @param error  에러 객체
         */
        virtual void error_occure(std::shared_ptr<void> socket ,std::shared_ptr<void> error) = 0;


    };

    /*!
     @breif 서버 베이스 클래스
     @detail 서버 베이스 클래스는 모든 종류의 서버 클래스는 이 클래스를 상속한 클래스다.
             서버 베이스 클래스를 상속한 모든 서버 클래스는 session_builder 핸들을 구현하여야 한다.
             또한 server_service_delegate 클래스를 상속한 클래스를 참조하여 연결 된 소켓 객체를 넘겨준다.
     @see server_service_delegate, session_base
     */
    class server_base {
    public:
        /*!
         @breif server_base 클래스 가상 소멸자
         */
        virtual ~server_base () {};

        /*
         @breif session_builder_type은 세션 빌더 핸들 타입이다.
         @detail 세션빌더 타입의 세션 빌더 함수 객체는 인자로 소켓 객체를 입력받고, 세션 객체를 생성하여 반환한다.
         세션빌더는 세션 팩토리 함수이다.
         */
        typedef std::function<std::shared_ptr<session_base>(std::shared_ptr<void>)> session_builder_type;
        
        /*!
         @defgroup server_base_needs_impl 상속한 클래스에서 구현해야하는 맴버 함수 인터페이스 그룹.
         @{
         */
       
        /*!
         @breif 클라이언트 연결 시작 인터페이스.
         @detail 이 클래스를 상속하는 클래스에서는 클라이언트로부터 오는 연결요청을 받아들이는 작업을 시작하도록 구현해야한다.
         */
        virtual void start_service() = 0;
        /*!
         @breif 클라이언트 연결 종료 인터페이스.
         @detail 이 클래스를 상속하는 클래스에서는 클라이언트로부터 오는 연결요청을 받아들이는 작업을 종료하도록 구현해야한다.
         */
        virtual void stop_service() = 0;
        /*!
         @breif 세션 빌더 함수 객체를 반환하는 인터페이스.
         @detail session_builder_type형 함수객체를 반환한다 이 함수객체에는 세션을 생성할 수 있는 구현이 포함되어 있다.
         */
        virtual session_builder_type get_session_builder () const = 0;
        
        /*!
         @}
         */ //end of server_base_needs_impl.
        
        
        /*!
         @breif server_service_delegate 계열 클래스 객체를 설정하는 setter 함수.
         @detail 이 함수를 통해 server_service_delegate 계열 객체를 설정하게 되면
                 server_base 계열 클래스는 해당 객체의 수명을 책임지게 된다.
         @param delegate delegate 객체
         */
        virtual void set_delegate(server_service_delegate* delegate)
        {
            delegate->set_target_server(this);
            _delegate.reset(delegate);
        }
        /*!
         @brief server_service_delegate 계열 클래스 객체 포인터를 가져오는 함수
         @detail server_service_delegate 계열 클래스 객체 포인터를 반환해도 server_base 계열 포인터가
                 server_service_delegate 수명 관리를 포기하지 않습니다.
         @return server_service_delegate* server_service_delegate 계열 클래스 객체 함수 포인터
         */
        virtual server_service_delegate* get_delegate() const
        {
            return _delegate.get();
        }
        

    protected:
        /*!
         @breif 기본 생성자
         @detail 상속한 클래스에서만 호출가능.
         */
        server_base() = default;
        
    private:

        /*!
         @breif delegate 객체 멤버 포인터
         @detail server_service_delegate 계열 클래스 객체 포인터를 std::unique_ptr가 감싸고 있다.
                 따라서 server_base 클래스가 메모리에서 해제 되었을 때, _delegate 포인터
                 주소에 할당된 server_service_delegate 객체는 할당해제 된다.
         */
        std::unique_ptr<server_service_delegate> _delegate;
    };

}
}
#endif /* server_base_h */
