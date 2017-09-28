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
         @ingroup server_base_require_impl
         @defgroup ssl_tcp_server_impl server_base 클래스로부터 상속되어 구현된 인터페이스 함수들.
         @{
         */
        //구현 클래스에 위임
        virtual void start_service()  {
            _pimpl->start_service();
        }
        virtual void stop_service() {
            _pimpl->stop_service();
        }
        virtual session_builder_type get_session_builder () const {
            return _pimpl->get_session_builder();
        }
        /*!
         @}
         */ //end of ssl_tcp_server_impl
        
        /*!
         @ingroup ssl_tcp_server_impl
         @defgroup ssl_tcp_server_impl_redef server_base의 구현을 재정의한 함수 그룹.
         @{
         */
        //pimpl 패턴을 위한 재정의.
        virtual void set_delegate(server_service_delegate* delegate) {
            _pimpl->set_delegate(delegate);
        }
        virtual server_service_delegate* get_delegate() const {
            return _pimpl->get_delegate();
        }
        /*!
         @}
         */ // end of ssl_tcp_server_impl_redef
        
    private:
        /*!
         @breif 구현 클래스의 객체 주소를 가리키는 pimpl.
         @detail 구현 클래스의 객체 주소를 unique_ptr로 wrapping 하였음.
                 구현 클래스의 객체 수명을 ssl_tcp_server 클래스가 구현 클래스 객체의 수명을 관리함.
                 ssl_tcp_server 클래스 객체가 소멸될 때 구현 클래스의 객체도 같이 소멸됨.
         */
        std::unique_ptr<server_base> _pimpl;
        
    };

}
}


#endif //pair of #ifndef SOLARCODE_LIVEMAP_TCP_SERVER
