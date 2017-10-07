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
@file solarcode_livemap_command.hpp
@breif 라이브 맵 명령 클래스.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 03. 06.
@version 0.0.1
*/
#ifndef SOLARCODE_LIVEMAP_COMMAND_FORM
#define SOLARCODE_LIVEMAP_COMMAND_FORM

#include "livemapserver_config.h"

#include <stdint.h>
#include <cstring>
#include <vector>
#include <iterator>
#include <memory>
#include <functional>
#include <stdint.h>

namespace solarcode {
namespace livemap {
    
    using command_type = uint32_t;
    const command_type DEFAULT_COMMAND_TYPE = UINT32_MAX;

     /*!
    @class command_form_base
    @brief 직렬화 도움 커맨드 클래스
    @detail n/a.
    @namespace solarcode::livemap::command_form_base;
    @see
    */
    template <command_type TYPE_ID = DEFAULT_COMMAND_TYPE>
    class command_form_base_t {
    public:
        struct segment_info {
            segment_info():begin(0),size(0){};
            
            segment_info(std::size_t input_begin, std::size_t input_size)
            :begin(input_begin),
            size(input_size) {}
            
            segment_info(segment_info& rhs)
            :begin(rhs.begin),
            size(rhs.size) {}
            
            segment_info& operator=(const segment_info& rhs)
            {
                if ( this == &rhs ) return *this;
                
                begin = rhs.begin;
                size = rhs.size;
                
                return *this;
            }
            
            std::size_t begin;
            std::size_t size;
        };
        /*!
        @breif 디폴트 생성자
        */
        command_form_base_t()
        :_segment_lastest_index(0),
        _segment_pool()
        {
            
        }
        
        command_form_base_t(const command_form_base_t& rhs)
        :_segment_lastest_index(rhs._segment_lastest_index),
        _segment_pool()
        {
            _segment_pool.clear();
            _segment_pool.assign(rhs._segment_pool.begin(), rhs._segment_pool.end());
        }
        
        
        command_form_base_t& operator=(const command_form_base_t& rhs)
        {
            
            if ( this == &rhs ) return *this;
            
            _segment_lastest_index = rhs._segment_lastest_index;
            _segment_pool.clear();
            _segment_pool.assign(rhs._segment_pool.begin(), rhs._segment_pool.end());
            
            return *this;
        }
        /*!
        @breif 이미 내용물이 있는 버퍼를 인자로 받는 생성자
        @param command 콜백 함수 
        */
        command_form_base_t(const char * const input_data, const std::size_t input_data_size)
        :_segment_lastest_index(input_data_size),
        _segment_pool()
        {
            std::copy(input_data, input_data + input_data_size, std::back_inserter(_segment_pool));
        }
        
        /*!
        @breif 세그먼트를 추가합니다.
        @param seg_buffer 추가할 세그먼트.
        @param seg_size_by_byte 세그먼트 사이즈
        */
        virtual segment_info add_segment(const void * const seg_buffer, const std::size_t seg_size_by_byte );
        
        /*!
        @breif 세그먼트 읽기
        @param seg_start 세그먼트 시작 인덱스
        @param seg_size_by_byte 세그먼트 사이즈
        @param read_buffer 읽어들일 세그먼트 버퍼 포인터 변수
        */
        virtual void read_segment(void * const read_buffer, segment_info target_segment);
        /*!
        @breif 전체 세그먼트 사이즈 얻기
        @return std::size 사이즈
        */
        virtual std::size_t get_entire_size();
        /*!
        @breif 세그먼트 완전히 지우기
        */
        virtual void clear();
        /*!
        @breif 세그먼트 직렬화 시키기
        @param serialized_buffer 직렬화된 버퍼를 읽어들일 포인터 변수
        */
        virtual std::size_t serialize(char * const serialized_buffer);

        /*!
        @breif 커맨드 타입 정보 저장.
         */
        enum { type = TYPE_ID  /*디폴트 값 32비트 정수 최대*/};
    private:
        /*!
        @breif 세그먼트 인덱스
        */
        std::size_t _segment_lastest_index;
        /*!
        @breif 세그먼트 배열
        */
        std::vector<char> _segment_pool;
    };

    using command_form_base = command_form_base_t<0>;
}
}

namespace solarcode {
namespace livemap {  	/*!@breif 세그먼트 데이터 추가 구현*/
	template <command_type TYPE_ID>
    typename command_form_base_t<TYPE_ID>::segment_info command_form_base_t<TYPE_ID>::add_segment(const void * const seg_buffer, const std::size_t seg_size_by_byte )
	{
        
        char input_data[seg_size_by_byte];
        
        std::memcpy(input_data, seg_buffer, seg_size_by_byte);
        
        std::copy(input_data, input_data + seg_size_by_byte, std::back_inserter(_segment_pool));

		std::size_t seg_start = _segment_lastest_index;

		_segment_lastest_index += seg_size_by_byte;
        
        segment_info info;
        info.begin = seg_start;
        info.size = seg_size_by_byte;
        return info;
	}
	/*!@breif 세그먼트 데이터를 읽는 구현*/
	template <command_type TYPE_ID>
	void command_form_base_t<TYPE_ID>::read_segment(void * const read_buffer, segment_info target_segment )
	{
		std::memcpy(read_buffer, _segment_pool.data() + target_segment.begin, target_segment.size);
	}
	/*!@breif 전체 세그먼트 사이즈를 계산하는 구현*/
	template <command_type TYPE_ID>
    std::size_t command_form_base_t<TYPE_ID>::get_entire_size()
	{
		return _segment_lastest_index;
	}

	/*!@breif 세그먼트를 지워 초기화하는 구현*/
	template <command_type TYPE_ID>
	void command_form_base_t<TYPE_ID>::clear()
	{
        _segment_pool.clear();
	}
	/*!@breif 직렬화하는 구현*/
	template <command_type TYPE_ID>
    std::size_t command_form_base_t<TYPE_ID>::serialize(char * const serialized_buffer)
	{

        std::memcpy(serialized_buffer, _segment_pool.data(), _segment_lastest_index);

        
        return _segment_lastest_index ;
	}
}
}
#endif
