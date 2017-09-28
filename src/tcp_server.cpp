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
    /*!
     생성자 구현.
     디폴트 구현 클래스는 ssl_tcp_server_boost_impl 클래스.
     */
    ssl_tcp_server::ssl_tcp_server()
    :server_base(),
    _pimpl(nullptr)
    {
        _pimpl.reset(new ssl_tcp_server_boost_impl());
        
    }
}
}

