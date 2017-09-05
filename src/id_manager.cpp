/*!
@file solarcode_id_manager.cpp
@breif id 발급 클래스 구현.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 19
@version 0.0.1
*/
#include "id_manager.hpp"

namespace solarcode {
namespace livemap {
    /*! @breif id 컨테이너 초기화 구현*/
    void local_id_manager::init_id_container() 
    {
        for ( uint32_t index = 0; index < _capacity; index++ ){

            _id_container.insert({index, false});
        }
        _cache_iter = _id_container.begin();
    }
    
    /*! @breif id 발급 요청하는 구현*/
    common_id_type local_id_manager::request_id()
    {
        //캐싱해둔 반복자 불러옴.
        auto iter = _cache_iter;
        
        //반복자가 컨테이너 끝을 가리키면
        if ( iter == (--_id_container.end())  ){
            if ( iter->second == false ) {
                //컨테이너 끝 id가 사용하고 있지 않다면 발급
                _id_container[iter->first] = true;
                //컨테이너 끝까지 왔으므로 처음으로 되돌림
                _cache_iter = _id_container.begin(); 
                //발급
                return iter->first;
            } else {
                //컨테이너 끝 id가 사용하고 있다면 발급 하지 않음.
                //반복자를 컨테이너 처음으로 돌림.
                iter = _id_container.begin();
            }

        }
        //발급할 id를 저장할 변수 초기 값 no_more_id
        common_id_type find_id = no_more_id;
        //루프를 빠져나오기 위한 플래그
        bool escape_flag = false;
        
        //반복자를 돌며 사용하고 있지 않는 id를 찾음.
        for ( ; iter !=_id_container.end() && escape_flag != true ; ++iter ) {
            
            if( iter->second == false ) {
                //사용중이지 않은 id를 찾았다면 발급 준비    
                find_id = iter->first;
                //마지막으로 찾은 위치 반복자 캐싱
                _cache_iter = iter; 
                //발급할 id를 결정했으므로 루프 탈출.
                escape_flag = true;
            }
            
        }
        
        // 발급한 id가 유효하다면 사용하고 있다고 set
        if( find_id != no_more_id ){
            _id_container[find_id] = true;
        }
        //발급
        return find_id;
        
    }
    
    /*! @breif id 반납하기*/
    void local_id_manager::return_id(common_id_type will_be_retured_id)
    {
        //반납할 id가 유효한지 확인.
        if( will_be_retured_id > _capacity ){
            return;//반납할 id가 유효하지 않다면 반납거부
        }
        //id 반납. 반납한 id는 사용하지 않으므로 unset
        _id_container[will_be_retured_id] = false;
    }
}
}
