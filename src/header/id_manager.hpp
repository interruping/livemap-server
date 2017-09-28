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
@file solarcode_id_manager.hpp
@breif id 발급 클래스 명세.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 19
@version 0.0.1
*/
#ifndef SOLARCODE_LIVEMAP_ID_MANAGER
#define SOLARCODE_LIVEMAP_ID_MANAGER

#include <stdint.h>
#include <map>

namespace solarcode{
namespace livemap {
    /*!
    @class id_manager_base
    @breif id 발급 매니저 베이스 클래스 
    @detail n/a.
    @namespace solarcode::livemap::id_manager_base
    @see 
    */
    template <typename id_type>
    class id_manager_base 
    {
    public:
        /*! @breif 기본 생성자 */
        id_manager_base() = default;
        /*! @brief 소멸자 */
        virtual ~id_manager_base(){};
        /*! @breif id 발급 요청 함수*/
        virtual id_type request_id() = 0;
        /*!
        @breif id 반납 함수 
        @param id 반납할 id 
        */
        virtual void return_id(id_type id) = 0;
        
    };
    /*!
    @class id_holder
    @breif id 소지자 베이스 클래스
    @detail n/a.
    @namespace solarcode::livemap::id_holder
    @see 
    */
    template <typename id_type>
    class id_holder {
    public:
    	id_holder() = default;
    	virtual ~id_holder(){}
        /*!
        @breif 아이디 세터
        */
       virtual void set_id(id_type id)
        {
            _id = id;
        }
        /*!
        @breif 아이디 게터
        */
       virtual id_type get_id() const
        {
            return _id;
        }
    private:
        /*!
        @breif 아이디 멤버 객체
        */
        id_type _id;
    };
}
}


namespace solarcode {
namespace livemap {
    /*! @breif 일반 id 타입*/
    using common_id_type = uint32_t;
    /*! @brief 일반 id 홀더 */
    using common_id_holder = id_holder<common_id_type>;
    /*! @breif id 발급 가능 수용량 타입*/
    using id_manager_capacity = uint32_t;

    enum {
            /*! @breif 최대 id 발급 수용량 상수 */
            id_manager_max_capacity = UINT32_MAX - 1,
            /*! @breif id가 바닥났을 때 id 대신 발급되는 값*/
            no_more_id = UINT32_MAX
    };
    
    
    /*!
    @class local_id_manager
    @breif 로컬 id 발급 매니저 클래스 
    @detail n/a.
    @namespace solarcode::livemap::local_id_manager
    @see 
    */    
    class local_id_manager: public id_manager_base<common_id_type> {
    public:
        /*!
        @breif 생성자
        @param capacity 수용량 
        */
        local_id_manager(id_manager_capacity capacity)
        :id_manager_base(),
        _capacity(capacity),
        _id_container(),
        _cache_iter(_id_container.begin())
        {
            init_id_container();
        }
        virtual ~local_id_manager(){}
                
        /*!
        @breif id 컨테이너 초기화
        */
        void init_id_container();
        /*!
        @breif id 발급 요청
        */
        common_id_type request_id();
        /*!
        @breif id 반납 요청
        @param will_retured_id 반납할 id
        */
        void return_id(common_id_type will_be_retured_id);
    private:
        /*! 
        @breif id 수용량
        */
        id_manager_capacity _capacity;
        /*!
        @breif 연결 완료된 클라이언트에게 발급할 id 컨테이너;
        */
        std::map<common_id_type, bool> _id_container;
        /*!
        @breif 컨테이너 반복자 캐싱을 위한 변수 
        */
        std::map<common_id_type, bool>::iterator _cache_iter;
        
        

    };
    

}
}


#endif
