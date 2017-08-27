//
//  server_base.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 15..
//
//

#ifndef server_base_h
#define server_base_h

#include <memory>
#include <functional>

namespace solarcode {
namespace livemap {
    
/*!
 @class tcp_server_delegate
 @breif tcp서버 delegate 매서드 가이드 추상 클래스
 @detail n/a.
 @namespace solarcode::livemap::tcp_server_delegate
 @see tcp_server
 */
class server_service_delegate  {
public:
    /*!
     @breif 클라이언트의 연결 요청을 수락이 완료된 후 소켓 객체의 사용을 위임하는 메서드.
     @param socket 소켓 객체
     */
    virtual void handle_accept(std::shared_ptr<void> socket) = 0;
    /*!
     @breif 클라이언트의 연결 요청을 수락하는 과정에서 에러가 발생했을 때 동작을 위임하는 매서드.
     @param socket 에러가 발생한 소켓 객체
     @param error  에러 객체
     */
    virtual void error_occure(std::shared_ptr<void> socket ,std::shared_ptr<void> error) = 0;

};

/*!
 @class server_base
 @breif 사버 베이스 클래스
 @detail n/a
 @namepace solarcode::livemap::tcp_server_base
 @see tcp_server_delegate
 */
class server_base {
public:
    
    virtual ~server_base () {};

    
    /*!
     \breif 클라이언 연결요청 accept 작업을 실행하는 동작을 구현할 매서드.
     */
    virtual void start_service() = 0;
    /*!
     \breif accept 작업을 종료하는 동작을 구현할 매서드.
     */
    virtual void stop_service() = 0;
    /*!
     @breif delegate 객체를 설정하는 함수.
     @param delegate delegate 객체 (약한 참조)
     */
    virtual void set_delegate(server_service_delegate* delegate)
    {
        _delegate.reset(delegate);
    }
    /*!
     @brief delegate 객체를 가져오는 함수
     @return
     */
    virtual server_service_delegate* get_delegate() const
    {
        return _delegate.get();
    }
    
    typedef std::function<std::shared_ptr<void>(std::shared_ptr<void>)> session_builder_type;
    
    virtual session_builder_type get_session_builder () const = 0;
protected:
    /*!
     @breif 생성자 (자식에서 만 호출가능)
     */
    server_base() = default;
    
private:

    /*!
     @breif delegate 객체 멤버 포인터
     */
    std::unique_ptr<server_service_delegate> _delegate;
};

}
}
#endif /* server_base_h */
