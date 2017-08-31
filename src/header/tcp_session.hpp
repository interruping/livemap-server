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

#include "debug_utility.hpp"
#include "session_base.hpp"


namespace solarcode {
namespace livemap {
    class ssl_tcp_session : public session_base {
    public:
        ssl_tcp_session(std::shared_ptr<void> socket);
        
        virtual ~ssl_tcp_session(){}
        
        virtual void start() {
            _pimple->start();
        }
        
        virtual void stop() {
            _pimple->stop();
        }
        
        virtual void set_delegate(session_io_delegate *delegate) {
            _pimple->set_delegate(delegate);
        }
        
        virtual session_io_delegate* get_delegate() const {
            return _pimple->get_delegate();
        }
        
        virtual std::shared_ptr<void> get_socket() const  {
            return _pimple->get_socket();
        }
        
        virtual void set_expire_callback(std::function<void()> expire_callback) {
            _pimple->set_expire_callback(expire_callback);
        }
        
        
    private:
        std::unique_ptr<session_base> _pimple;
    };
    
    
    
}
}

#endif
