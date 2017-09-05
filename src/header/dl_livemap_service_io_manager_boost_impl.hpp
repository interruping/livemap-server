//
//  dl_livemap_service_io_manager_boost_impl.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#ifndef dl_livemap_service_io_manager_boost_impl_h
#define dl_livemap_service_io_manager_boost_impl_h
#include "boost_asio_helper.hpp"
#include "session_io_manager_base.hpp"
#include "client_node_pool.hpp"

namespace solarcode {
namespace livemap {
    class dl_livemap_service_io_manager_boost_impl : public session_io_manager_base {
    public:
        dl_livemap_service_io_manager_boost_impl(std::weak_ptr<session_base> session, client_node_pool& node_pool);
        
        ~dl_livemap_service_io_manager_boost_impl(){}

        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length);
        virtual std::size_t session_write_before_buffer(char ** buffer);
    private:
        static boost::asio::strand _strand_for_node_pool;
        
        boost::asio::io_service& _io_service;
        
        client_node_pool& _node_pool;
        
    };
}
}

#endif /* dl_livemap_service_io_manager_boost_impl_h */
