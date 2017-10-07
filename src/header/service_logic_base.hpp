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
 @file service_logic_base.hpp
 @author GeunYoung Lim, interruping4dev@gmail.com
 @date 2017. 08. 30.
 */

#ifndef service_logic_base_h
#define service_logic_base_h

#include "livemapserver_config.h"

#include "server_base.hpp"

#include <memory>

namespace solarcode {
namespace livemap {

    /*!
     @breif 서버 구동 로직 베이스 클래스.
     @details server_base 클래스 계열 객체로부터 연결받은 소켓을 처리, 세션을 생성하는 로직을 구현하기 위한 베이스 클래스.
     */
    class service_logic_base : public server_service_delegate {
    public:
        /*!
         @breif 가상 소멸자.
         */
        virtual ~service_logic_base(){
            
        }
        /*!
         @ingroup server_service_delegate_require_impl
         @defgroup service_logic_base_require_impl server_logic_base 클래스의 서브클래스가 구현해야하는 인터페이스 함수들.
         @{
         */
        virtual void set_target_server(server_base *target_server) = 0;
        virtual void handle_accept(std::shared_ptr<void> socket) = 0;
        virtual void error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error) = 0;
        /*!
         @}
         */
    protected:
        /*!
         @breif 보호된 생성자.
         @details 외부에서 직접적으로 이 클래스의 인스턴스를 생성할 수 없도록 한다.
         */
        service_logic_base()
        :server_service_delegate()
        {
            
        }
    };
}
}

#endif /* service_logic_base_h */
