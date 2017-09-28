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
 @file repeat_io_manager.hpp
 @author GeunYoung Lim, interruping4dev@gmail.com
 @date 2017. 8. 31.
 */

#ifndef repeat_io_manager_h
#define repeat_io_manager_h

#include <vector>

#include "session_io_manager_base.hpp"

namespace solarcode {
namespace livemap {
    /*!
     @breif 반복 입출력 세션 매니저
     @details 세션에서 읽은 데이터를 그대로 세션에게 다시 전달한는 세션 매니저.
     */
    class repeat_io_manager : public session_io_manager_base {
    public:
        /*!
         @breif 생성자
         @details 슈퍼클래스 session_io_manager_base 클래스의 생성자에게 넘겨줄 세션 객체 인자로 받는다.
         @see session_io_manager::session_io_manager(std::weak_ptr<session_base> session)
         @param session session_base 클래스 계역 객체 weak 포인터
         */
        repeat_io_manager(std::weak_ptr<session_base> session)
        :session_io_manager_base(session),
        _repeatBuffer()
        {

        }
        
        virtual ~repeat_io_manager() {};
        
        /*!
         @ingroup session_io_manager_require_impl
         @defgruop repeat_io_manager_impl repeat_io_manager 클래스가 session_io_manager_base에서 상속하여 구현한 인터페이스 함수들.
         @{
         */
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length);
        virtual std::size_t session_write_before_buffer(char ** buffer);
        /*!
         @}
         */
    private:
        /*!
         @breif 데이터 버퍼
         @details 세션 객체에서 읽은 데이터를 임시로 저장하는 버퍼.
         */
        std::vector<char> _repeatBuffer;

    };
}
}

#endif /* repeat_io_manager_h */
