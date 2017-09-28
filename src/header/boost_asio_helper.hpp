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

#ifndef boost_asio_helper_h
#define boost_asio_helper_h

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace solarcode {
namespace livemap {
    boost::asio::io_service& get_shared_io_service();
    
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> bst_ssl_tcp_socket;
}
}
#endif /* boost_asio_helper_h */
