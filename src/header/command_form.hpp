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

#include <stdint.h>
#include <cstring>

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
        enum {
            //세그먼트 최대 사이즈 512바이트
            SEGMENT_ARRAY_MAX = 512
        };
        /*!
        @breif 디폴트 생성자
        */
        command_form_base_t()
        :_segment_lastest_index(0),
        _segment_array()
        {
            
        }
        
        /*!
        @breif 이미 내용물이 있는 버퍼를 인자로 받는 생성자
        @param command 콜백 함수 
        */
        command_form_base_t(const char * const buffer_512_byte)
        :_segment_lastest_index(0),
        _segment_array()
        {
            std::memcpy(_segment_array, buffer_512_byte, 512);

        }
        
        /*!
        @breif 세그먼트를 추가합니다.
        @param seg_buffer 추가할 세그먼트.
        @param seg_size_by_byte 세그먼트 사이즈
        */
        virtual std::size_t add_segment(void * const seg_buffer, const std::size_t seg_size_by_byte );
        
        /*!
        @breif 세그먼트 읽기
        @param seg_start 세그먼트 시작 인덱스
        @param seg_size_by_byte 세그먼트 사이즈
        @param read_buffer 읽어들일 세그먼트 버퍼 포인터 변수
        */
        virtual void read_segment(const std::size_t seg_start, const std::size_t seg_size_by_byte, void * const read_buffer);
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
        virtual void serialize(char *const serialized_buffer);

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
        char _segment_array[SEGMENT_ARRAY_MAX];
    };

    using command_form_base = command_form_base_t<>;
}
}

namespace solarcode {
namespace livemap {
	/*!@breif 세그먼트 데이터 추가 구현*/
	template <command_type TYPE_ID>
	std::size_t command_form_base_t<TYPE_ID>::add_segment(void * const seg_buffer, const std::size_t seg_size_by_byte )
	{

		std::memcpy(_segment_array + _segment_lastest_index, seg_buffer, seg_size_by_byte);

		std::size_t seg_start = _segment_lastest_index;

		_segment_lastest_index += seg_size_by_byte;

		return seg_start;
	}
	/*!@breif 세그먼트 데이터를 읽는 구현*/
	template <command_type TYPE_ID>
	void command_form_base_t<TYPE_ID>::read_segment(const std::size_t seg_start, const std::size_t seg_size_by_byte, void * const read_buffer)
	{
		std::memcpy(read_buffer, _segment_array + seg_start, seg_size_by_byte);
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
		std::memset(_segment_array, 0, _segment_lastest_index);
		_segment_lastest_index = 0;
	}
	/*!@breif 직렬화하는 구현*/
	template <command_type TYPE_ID>
	void command_form_base_t<TYPE_ID>::serialize(char *const serialized_buffer)
	{
		std::memcpy(serialized_buffer, _segment_array, _segment_lastest_index);
	}
}
}
#endif
