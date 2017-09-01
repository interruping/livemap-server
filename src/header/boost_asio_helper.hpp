//
//  boost_asio_helper.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 26..
//
//

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
