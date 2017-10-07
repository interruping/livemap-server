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
 @file livemap_service_logic.hpp
 @author GeunYoung Lim, interruping4dev@gmail.com
 @date 2017. 9. 1.
 */
#ifndef livemap_service_logic_h
#define livemap_service_logic_h

#include "livemapserver_config.h"

#include <mutex>

#include "service_logic_base.hpp"
#include "id_manager.hpp"
#include "client_node_pool.hpp"

namespace solarcode {
namespace livemap {
    /*!
     @breif 라이브맵 서비스 로직 클래스
     @detail 소켓과 세션을 다루고 세션에 서비스를 연결해주는 클래스.
     @see service_logic_base
     */
    class livemap_service_logic : public service_logic_base {
    public:
        /*!
         @breif 생성자
         */
        livemap_service_logic()
        :service_logic_base()
        , _session_builder()
        , _id_manager(100)
        , _node_pool()
        {
            
        }
        /*!
         @breif 소멸자
         */
        virtual ~livemap_service_logic() {
            
        }
        /*!
         @ingroup service_logic_base_require_impl
         @defgroup livemap_service_logic_impl server_logic_base 클래스로부터 상속받아 구현한 인터페이스 함수들.
         @{
         */
        virtual void set_target_server(server_base *target_server);
        virtual void handle_accept(std::shared_ptr<void> socket);
        virtual void error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error);
        /*!
         @}
         *///end of service_logic_base_require_impl
    private:
        /*!
         @breif 세션 빌더 핸들
         @details 소켓을 인자로 받고 세션을 생성하는 세션 빌더 함수 객체.
         @see server_base::session_builder_type
         */
        server_base::session_builder_type _session_builder;
        /*!
         @breif 스레드 동기화를 위한 mutex
         @details id를 발급하는 _id_manager 객체와 사용자 노드 관리 객체인 _node_pooål을 스레드 동기화한다.
         */
        std::mutex _mutex_for_id_manager_and_node_pool;
        /*!
         @breif id 매니저
         @details 새로 생성된 세션 객체의 소유 client_node에 id를 발급해준다.
                  만료된 세션은 세션의 client_node로 부터 처음 부여받았던 id를 반납한다.
                  id의 발급과 반납은 서로 다른 스레드에서 실행 될 수 있다.
         */
        local_id_manager _id_manager;
        
        /*!
         @breif node pool
         @details 사용자 노드를 관리하는 객체.
         */
        client_node_pool _node_pool;

    };
}
}


#endif /* livemap_service_logic_h */
