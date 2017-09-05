//
//  dl_livemap_service_io_manager_boost_impl.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#include "dl_livemap_service_io_manager_boost_impl.hpp"

namespace solarcode {
namespace livemap {
    dl_livemap_service_io_manager_boost_impl::dl_livemap_service_io_manager_boost_impl(std::weak_ptr<session_base> session, client_node_pool& node_pool)
    :solarcode::livemap::session_io_manager_base(session),
    _io_service(get_shared_io_service()),
    _node_pool(node_pool)
    {
        
        
        
    }
    
    
    boost::asio::strand
    dl_livemap_service_io_manager_boost_impl::_strand_for_node_pool
    = boost::asio::strand(get_shared_io_service());


}
}
