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
