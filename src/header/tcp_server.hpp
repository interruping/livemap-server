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

#include <memory>

namespace solarcode {
namespace livemap {
    class ssl_tcp_server : server_base {
    public:
        ssl_tcp_server();
        
        virtual void start_accept()  {
            _pimple->start_service();
        }
        virtual void stop_service() {
            _pimple->stop_service();
        }
        virtual void set_delegate(server_service_delegate* delegate) {
            _pimple->set_delegate(delegate);
        }
        
        virtual server_service_delegate* get_delegate() const {
            return _pimple->get_delegate();
        }
        
        virtual session_builder_type get_session_builder () const {
            return _pimple->get_session_builder();
        }
        
    private:
        std::unique_ptr<server_base> _pimple;
    };

}
}


#endif //pair of #ifndef SOLARCODE_LIVEMAP_TCP_SERVER
