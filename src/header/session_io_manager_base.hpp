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

//
//  session_io_manager_base.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 30..
//
//

#ifndef session_io_manager_base_h
#define session_io_manager_base_h

#include <memory>
#include "session_base.hpp"
#include "client_node.hpp"
#include "debug_utility.hpp"

namespace solarcode {
namespace livemap {
    /*!
     @breif 세션 입출력 관리 베이스 클래스.
     @details 세션의 입출력에 사용되는 데이터를 관리하는 베이스 클래스.
              이 클래스의 객체는 세션을 소유하고 있는 고유의 클라이언트 객체를 참조한다.
     */
    class session_io_manager_base : public session_io_delegate {
    public:
        /*!
         @breif 생성자.
         @details session_io_manager_base 클래스 계열의 객체는 세션 객체의 입출력을 관리하므로
                  필수로 하나의 세션 객체를 참조하여햐 한다. 따라서 생성자에서 세션 객체의 참조를 인자로 받는다.
                  session_io_manager_base 객체는 세션의 수명관리 책임이 없다. 따라서 세션을 약하게 참조한다(메모리 소유권을 주장하지 않음)
         
         @param session 세션 객체의 weak 포인터.
         */
        session_io_manager_base(std::weak_ptr<session_base> session)
        : session_io_delegate()
        , _session(session)
        , _owner(nullptr)
        {
            std::shared_ptr<session_base> target_session = session.lock();
            
            if ( target_session ) {
                target_session->set_delegate(this);
            }
        }
        
        /*!
         @breif 소멸자
         */
        virtual ~session_io_manager_base() {
#ifdef _DEBUG_
            SC_DBGMSG("session_io_manager_base destructor execute.");
#endif
        }
        
        /*!
         @ingroup session_io_delegate_require_impl
         @group session_io_manager_base_require_impl session_io_manager_base 클래스의 서브클래스가 구현해야하는 인터페이스 함수들.
         @{
         */
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length) = 0;
        virtual std::size_t session_write_before_buffer(char **buffer) = 0;
        /*!
         @}
         */
        
        /*!
         @breif client_node 객체 setter
         @details 세션을 소유(점유)하고 있는 client_node 객체를 참조하기 위한 setter.
         @params owner client_node 객체.
         */
        virtual void set_session_owner(std::shared_ptr<client_node> owner) {
            _owner = owner;
        }
        
        /*!
         @breif 참조하고 client_node 포인터 반환 getter.
         @details client_node는 여러 객체에서 참조되므로 session_io_manager_base가 직접적으로 수명관리를 하지 않는다.
                  따라서 weak 포인터로 반환한다. 이 함수를 호출하는 사용자는 client_node 객체의 메모리 해제여부를 확인해야 한다.
         @return std::weak_ptr<client_node> client_node 객체 weak 포인터.
         */
        virtual std::weak_ptr<client_node> get_session_owner() const {
            return _owner;
        }
        
        /*!
         @brief 참조하고 있는 세션객체 session_base 포인터 반환 getter.
         @details 입출력을 관리하고 있는 session_base 객체의 약한 참조 포인터를 반환한다.
         @return std::weak_ptr<session_base> session_base 객체 weak 포인터.
         */
        virtual std::weak_ptr<session_base> get_session() const
        {
            return _session;
        }
        
    private:
        /*!
         @breif session_base 클래스 계열 객체 weak 포인터
         @details 세션 객체의 메모리를 수명 관리를 하지 않기때문에 weak 포인터로 참조한다.
         */
        std::weak_ptr<session_base> _session;
        /*!
         @breif client_node 객체 포인터
         @details client_node 객체는 여러 다른 클래스 객체에서 참조하므로 직접적인 메모리 수명 관리를 하지 않는다.
                  따라서 참조 카운터 포인터로 참조한다.
         */
         std::shared_ptr<client_node> _owner;
    };
}
}
#endif /* session_io_manager_base_h */
