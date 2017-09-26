/*!
 @file solarcode_tcp_server.hpp
 @date 2017. 02. 05
 @author GeunYoungLim, interruping4dev@gmail.com
*/
#ifndef SOLARCODE_LIVEMAP_TCP_SERVER
#define SOLARCODE_LIVEMAP_TCP_SERVER

#include "server_base.hpp"

#include <memory>

namespace solarcode {
namespace livemap {
    /*!
     @brief ssl 소켓 서버
     @detail serer_base 클래스를 상속한 클래스.
             ssl 소켓으로 클라이언트의 연결을 받아들인다.
             내부에 구현이 있지 않고 구현 클래스는 따로 있다.
             구현 클래스는 pimpl 패턴으로 이 클래스와 같이 사용됨.
     */
    class ssl_tcp_server : public server_base {
    public:
        /*!
         @breif ssl 소켓 서버 생성자.
         */
        ssl_tcp_server();
        
        /*!
         @defgroup server_base로 상속받은 매서드들.
         @{
         */
        //구현 클래스에 위임
        virtual void start_service()  {
            _pimpl->start_service();
        }
        virtual void stop_service() {
            _pimpl->stop_service();
        }
        virtual void set_delegate(server_service_delegate* delegate) {
            _pimpl->set_delegate(delegate);
        }
        virtual server_service_delegate* get_delegate() const {
            return _pimpl->get_delegate();
        }
        virtual session_builder_type get_session_builder () const {
            return _pimpl->get_session_builder();
        }
        /*!
         @}
         */
    private:
        /*!
         @breif 구현 클래스의 주소를 가리키는 pimpl.
         */
        std::unique_ptr<server_base> _pimpl;
        
    };

}
}


#endif //pair of #ifndef SOLARCODE_LIVEMAP_TCP_SERVER
