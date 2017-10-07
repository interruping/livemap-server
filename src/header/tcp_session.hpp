// Copyright 2017 GeunYoung Lim <interruping4dev@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 @file tcp_session.hpp
 @breif tcp 계열 세션 모음.
 @detail ssl 소켓 세션 클래스가 선언되어있다.
         추후 tcp 세션 클래스를 추가할 예정이다.
 @author GeunYoung Lim, interruping@naver.com
 @data 2017. 02. 09
*/

#ifndef SOLARCODE_LIVEMAP_TCP_SESSION
#define SOLARCODE_LIVEMAP_TCP_SESSION

#include "livemapserver_config.h"

#include "debug_utility.hpp"
#include "session_base.hpp"


namespace solarcode {
namespace livemap {
    /*!
     @breif ssl 소켓 세션 클래스
     @detail ssl 소켓을 이용하여 클라이언트와 통신하는 클래스.
             pimpl패턴으로 구현되어 별도의 구현 클래스로 상세 구현한다.
             디폴트 구현 클래스는 ssl_tcp_session_boost_impl 클래스.
     @see ssl_tcp_session_boost_impl
     */
    class ssl_tcp_session : public session_base {
    public:
        ssl_tcp_session(std::shared_ptr<void> socket);
        
        
        virtual ~ssl_tcp_session(){}
        
        /*!
         @ingroup session_base_require_impl
         @defgroup ssl_tcp_session_impl session_base로부터 상속받아 구현된 인터페이스 함수들.
         @{
         */
        //pimpl 패턴 구현.
        virtual void start() {
            _pimpl->start();
        }
        
        virtual void stop() {
            _pimpl->stop();
        }
        /*!
         @}
         */ //end of ssl_tcp_session_impl
        
        /*!
         @ingroup ssl_tcp_session_impl
         @defgroup ssl_tcp_session_imp_redef session_base의 구현을 재정의한 함수들.
         @{
         */
        //pimpl 패턴을 위한 재정의.
        virtual void set_delegate(session_io_delegate *delegate) {
            _pimpl->set_delegate(delegate);
        }
        
        virtual session_io_delegate* get_delegate() const {
            return _pimpl->get_delegate();
        }
        
        virtual std::shared_ptr<void> get_socket() const  {
            return _pimpl->get_socket();
        }
        
        virtual void set_expire_callback(std::function<void()> expire_callback) {
            _pimpl->set_expire_callback(expire_callback);
        }
        /*!
         @}
         */ //end of ssl_tcp_session_impl_redef
        
        
    private:
        /*!
         @breif 구현 클래스의 객체 포인터
         @details unique_ptr로 wrapping 됨.
                  ssl_tcp_session 클래스가 구현클래스의 수명을 관리한다.
                  ssl_tcp_session 클래스가 소멸되면 구현클래스도 소멸된다.
         */
        std::shared_ptr<session_base> _pimpl;
    };
    
}
}

#endif
