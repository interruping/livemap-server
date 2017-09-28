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


#ifndef SOLARCODE_LIVEMAP_CLIENT_NODE
#define SOLARCODE_LIVEMAP_CLIENT_NODE


#include "id_manager.hpp"

#include <stdint.h>
#include <set>
#include <string>
#include <iostream>
#include <memory.h>

#ifdef _DEBUG_
#include "debug_utility.hpp"
#endif

namespace solarcode {
namespace livemap {
    /*! 
    @breif 좌표 구조체
    */
    typedef struct coordinate {
        double latitude = 0.0f; //위도
        double longitude = 0.0f; //경도
    } coordinate;
    
    /*! 
    @breif 좌표 팩토리 함수
    @param latitude 위도
    @param longitude 경도 
    */
    inline coordinate make_coordinate(double latitude, double longitude) 
    {
        coordinate loc;
        loc.latitude = latitude;
        loc.longitude = longitude;
        
        return loc;
    }
    
    /*! 
    @breif 좌표 팩토리 함수 (C 스트링 버전)
    @param latitude 위도
    @param latitude 경도
    */
    inline coordinate make_coordinate(const char *latitude, const char *longitude) 
    {
        return make_coordinate(std::stod(std::string(latitude)),std::stod(std::string(longitude)));
    }
    /*! 
    @breif 좌표 팩토리 함수 (스트링 버전)
    @param latitude 위도
    @param latitude 경도
    */
    inline coordinate make_coordinate(std::string& latitude, std::string& longitude) 
    {
        return make_coordinate(std::stod(latitude),std::stod(longitude));
    }
    
    /*! 
    @breif 좌표 팩토리 함수 (우 참조 스트링 버전)
    @param latitude 위도
    @param latitude 경도
    */
    inline coordinate make_coordinate(std::string&& latitude, std::string&& longitude) 
    {
        return make_coordinate(std::stod(latitude),std::stod(longitude));
    }
}
}



namespace solarcode {
namespace livemap {

    class client_node : public id_holder<common_id_type> {
    public:

        /*!
        @breif 기본 생성자
        */
        client_node() = delete;
        /*!
        @breif 클라이언트 노드의 ID를 매개변수로 받는 생성자
        @param unique_id 노드 아이디
        */
        client_node(common_id_type unique_id);
        
        client_node(const client_node& rhs);
        client_node& operator=(const client_node& rhs);
        /*!
        @breif 소멸자
        */
        virtual ~client_node();

        /*!
        @breif 노드의 좌표 정보를 업데이트(설정)한다.
        @param location 좌표 정보가 담긴 구조체. (참조 형 파라미터)
        */
        virtual void set_coordinate(const coordinate& location);
        virtual void set_coordinate(const coordinate&& location); ///@breif 우측 참조 파라미터 버전
        /*!
        @breif 노드의 좌표 정보를 가져온다. 
        @return coordinate 좌표 정보 반환.
        */
        virtual const coordinate getCoordinate() const;

    private:
        /*! @breif 위도 변수 */
        double _latitude;
        /*! @breif 경도 변수 */
        double _longitude;

    };
}
}

#endif //#ifndef SOLARCODE_LIVEMAP_CLIENT_NODE
