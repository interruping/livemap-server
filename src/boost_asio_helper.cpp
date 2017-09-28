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

//
//  boost_asio_helper.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 31..
//
//

#include "boost_asio_helper.hpp"

namespace solarcode {
 namespace livemap {
     boost::asio::io_service& get_shared_io_service() {
         static boost::asio::io_service *shared_ios = nullptr;
         
         if ( shared_ios == nullptr ) {
             shared_ios = new boost::asio::io_service();
         }
         
         return *shared_ios;
     }
     
}
}
