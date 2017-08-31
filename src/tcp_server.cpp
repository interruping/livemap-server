/*!
@file solarcode_tcp_server.cpp
@breif tcp 서버 관련 구현 클래스.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 09
@version 0.0.1
*/

#include "tcp_server.hpp"
#include "ssl_tcp_server_boost_impl.hpp"

namespace solarcode {
namespace livemap {
    ssl_tcp_server::ssl_tcp_server()
    :server_base(),
    _pimple(nullptr)
    {
        _pimple.reset(new ssl_tcp_server_boost_impl());
        
    }
}
}

