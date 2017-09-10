#ifdef _DEBUG_
#include "debug_utility.hpp"
#endif

#include "client_node.hpp"
#include <iostream>
#include <memory.h>

namespace solarcode {
namespace livemap {
    
    /*! @breif 커스텀 생성자 구현 */
    client_node::client_node(common_id_type unique_id)
    :id_holder<common_id_type>(),
	 _latitude(0.0f), //경도 초기화
    _longitude(0.0f) //위도 초기화
    {
        set_id(unique_id);
    }
    
    /*! @breif 소멸자 구현 */
    client_node::~client_node()
    {
        
    }
}
}


namespace solarcode {
namespace livemap {


        /*!
        @breif 노드의 좌표 정보를 업데이트(설정)한다.
        */
        void client_node::setCoordinate(const coordinate& location)
        {
            _latitude = location.latitude;
            _longitude = location.longitude;
        }
        /*!
        @breif 노드의 좌표 정보를 업데이트(설정)한다.
        */
        void client_node::setCoordinate(const coordinate&& location)
        {
#ifdef _DEBUG_
            SC_DBGMSG("input coordinate info: lat - " << location.latitude  << "lon - " << location.longitude );
#endif

            _latitude = location.latitude;
            _longitude = location.longitude;
        }
        /*!
        @breif 노드의 좌표 정보를 가져온다. 
        */
        const coordinate client_node::getCoordinate() const
        {
#ifdef _DEBUG_
            SC_DBGMSG("return coordinate");
#endif
            return make_coordinate(_latitude, _longitude);
        }
        
        
}
}
